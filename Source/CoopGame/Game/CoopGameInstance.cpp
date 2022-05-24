// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CoopGame/UI/MainMenuWidget.h"
#include "CoopGame/UI/IngameMenuWidget.h"

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
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Subsystem: ") + Subsystem->GetSubsystemName().ToString() + TEXT(" Platform: ") + Subsystem->GetLocalPlatformName());
		}
		SessionInterface = Subsystem->GetSessionInterface();
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
		SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
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
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num() > 0)
		{
			TArray<FString> ServerNames;
			for (const FOnlineSessionSearchResult& Result : SearchResults)
			{
				ServerNames.Add(Result.GetSessionIdStr());
			}
			MainMenu->SetServerList(ServerNames);
		}
	}
}

void UCoopGameInstance::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		FString Address;
		if (SessionInterface->GetResolvedConnectString(InSessionName, Address))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Connect to ")+ Address);
			}
			APlayerController* PC = GetFirstLocalPlayerController();
			if (PC)
			{
				PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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

void UCoopGameInstance::Host()
{
	if (SessionInterface)
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SessionName);
		}
		else
		{
			CreateSession(SessionName);
		}
	}
}

void UCoopGameInstance::Join(uint32 Index)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Joining ") + Index);
	}
	if (SessionInterface && SessionSearch)
	{
		SessionInterface->JoinSession(0, SessionName, SessionSearch->SearchResults[Index]);
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