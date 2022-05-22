// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "CoopGame/UI/MainMenuWidget.h"
#include "CoopGame/UI/IngameMenuWidget.h"
#include "GameFramework/GameUserSettings.h"

UCoopGameInstance::UCoopGameInstance(const FObjectInitializer& ObjectIniyializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/MainMenu/WBP_MainMenu"));
	if (MenuBPClass.Class != nullptr)
	{
		MenuClass = MenuBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> IngameMenuBPClass(TEXT("/Game/UI/IngameMenu/WBP_IngameMenu"));
	if (IngameMenuBPClass.Class != nullptr)
	{
		IngameMenuClass = IngameMenuBPClass.Class;
	}
}

void UCoopGameInstance::Init()
{
}

void UCoopGameInstance::LoadMenu()
{
	if (MenuClass)
	{
		UMainMenuWidget* Menu = CreateWidget<UMainMenuWidget>(this, MenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Hosting Server"));
	}
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Map/NewMap?listen");
	}
}

void UCoopGameInstance::Join(const FString& Address)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Joining ") + Address);
	}
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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