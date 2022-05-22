// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClick_HostButton);
	}
	if (JoinMenuButton)
	{
		JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClick_JoinMenuButton);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClick_QuitButton);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClick_CancelButton);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClick_JoinButton);
	}
	return true;
}

void UMainMenuWidget::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
	MenuInterface = NewMenuInterface;
}

void UMainMenuWidget::Setup()
{
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(this->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputModeData);
			PC->bShowMouseCursor = true;
		}
	}
}

void UMainMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	this->RemoveFromViewport();
	APlayerController* PC = InWorld->GetFirstPlayerController();
	if (PC)
	{
		FInputModeGameOnly InputModeData;
		PC->SetInputMode(InputModeData);
		PC->bShowMouseCursor = false;
	}
}

void UMainMenuWidget::OnClick_HostButton()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenuWidget::OnClick_JoinMenuButton()
{
	if (MenuSwitcher)
	{
		if (JoinMenu)
		{
			MenuSwitcher->SetActiveWidget(JoinMenu);
		}
	}
}

void UMainMenuWidget::OnClick_QuitButton()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			PC->ConsoleCommand("quit");
		}
	}
}

void UMainMenuWidget::OnClick_CancelButton()
{
	if (MenuSwitcher)
	{
		if (MainMenu)
		{
			MenuSwitcher->SetActiveWidget(MainMenu);
		}
	}
}

void UMainMenuWidget::OnClick_JoinButton()
{
	if (MenuInterface)
	{
		FString IP;
		if (IPAddressText)
		{
			IP = IPAddressText->GetText().ToString();
		}
		if (!IP.IsEmpty())
		{
			MenuInterface->Join(IP);
		}
	}
}