// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CoopGame/UI/MainMenuWidget.h"
#include "CoopGame/UI/IngameMenuWidget.h"

const static FName SERVER_NAME_SETTING_KEY = TEXT("ServerName");

UCoopGameInstance::UCoopGameInstance(const FObjectInitializer& ObjectIniyializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/MainMenu/WBP_MainMenu"));
	if (MainMenuBPClass.Class != nullptr)
	{
		MainMenuClass = MainMenuBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> IngameMenuBPClass(TEXT("/Game/UI/IngameMenu/WBP_IngameMenu"));
	if (IngameMenuBPClass.Class != nullptr)
	{
		IngameMenuClass = IngameMenuBPClass.Class;
	}
}

void UCoopGameInstance::Init()
{
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Subsystem: ") + OnlineSubsystem->GetSubsystemName().ToString() + TEXT(" Platform: ") + OnlineSubsystem->GetLocalPlatformName());
		}
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCoopGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCoopGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCoopGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCoopGameInstance::OnJoinSessionComplete);
		}
	}
}

void UCoopGameInstance::RefreshServerList()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("RefreshServerList"));
	}
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		//SessionSearch->bIsLanQuery = true;
		// make sure can find session of this game, because we're using shared AppID
		SessionSearch->MaxSearchResults = 1000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCoopGameInstance::CreateSession(FName InSessionName)
{
	if (SessionInterface)
	{
		FOnlineSessionSettings SessionSettings;
		if (OnlineSubsystem && OnlineSubsystem->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_NAME_SETTING_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, InSessionName, SessionSettings);
	}
}

void UCoopGameInstance::OnCreateSessionComplete(FName InSessionName, bool IsSuccess)
{
	if (IsSuccess)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/Map/Lobby?listen");
		}
	}
}

void UCoopGameInstance::OnDestroySessionComplete(FName InSessionName, bool IsSuccess)
{
	if (IsSuccess)
	{
		CreateSession(InSessionName);
	}
}

void UCoopGameInstance::OnFindSessionComplete(bool IsSuccess)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("OnFindSessionComplete"));
	}
	if (IsSuccess && SessionSearch && MainMenu)
	{
		TArray<FServerData> ServerDatas;
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num() > 0)
		{
			for (const FOnlineSessionSearchResult& Result : SearchResults)
			{
				FServerData Data;
				Data.HostUserName = Result.Session.OwningUserName;
				Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
				FString ServerName;
				if (Result.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName))
				{
					Data.ServerName = ServerName;
				}
				else
				{
					Data.ServerName = Result.GetSessionIdStr();
				}
				ServerDatas.Add(Data);
			}
		}
		MainMenu->SetServerList(ServerDatas);
	}
}

void UCoopGameInstance::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		FString Address;
		if (SessionInterface->GetResolvedConnectString(InSessionName, Address))
		{
			APlayerController* PC = GetFirstLocalPlayerController();
			if (PC)
			{
				PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Connect to ") + Address);
			}
		}
	}
}

void UCoopGameInstance::LoadMainMenu()
{
	if (MainMenuClass)
	{
		MainMenu = CreateWidget<UMainMenuWidget>(this, MainMenuClass);
		if (MainMenu)
		{
			MainMenu->Setup();
			MainMenu->SetMenuInterface(this);
		}
	}
}

void UCoopGameInstance::LoadIngameMenu()
{
	if (IngameMenuClass)
	{
		UIngameMenuWidget* IngameMenu = CreateWidget<UIngameMenuWidget>(this, IngameMenuClass);
		if (IngameMenu)
		{
			IngameMenu->Setup();
			IngameMenu->SetMenuInterface(this);
		}
	}
}

void UCoopGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface)
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else
		{
			CreateSession(NAME_GameSession);
		}
	}
}

void UCoopGameInstance::Join(int32 Index)
{
	if (SessionInterface && SessionSearch && SessionSearch->SearchResults.Num() > 0 && Index < SessionSearch->SearchResults.Num())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Joining ") + SessionSearch->SearchResults[Index].GetSessionIdStr());
		}
		SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Cant join "));
		}
	}
}

void UCoopGameInstance::Leave()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Leave"));
	}
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel("/Game/UI/MainMenu/MainMenu", ETravelType::TRAVEL_Absolute);
	}
}