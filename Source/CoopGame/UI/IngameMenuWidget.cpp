// Fill out your copyright notice in the Description page of Project Settings.

#include "IngameMenuWidget.h"
#include "Components/Button.h"

bool UIngameMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UIngameMenuWidget::OnClick_CancelButton);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UIngameMenuWidget::OnClick_QuitButton);
	}
	bIsFocusable = true;
	return true;
}

void UIngameMenuWidget::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
	MenuInterface = NewMenuInterface;
}

void UIngameMenuWidget::Setup()
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

void UIngameMenuWidget::OnClick_CancelButton()
{
	this->RemoveFromViewport();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData);
			PC->bShowMouseCursor = false;
		}
	}
}

void UIngameMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
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

void UIngameMenuWidget::OnClick_QuitButton()
{
	if (MenuInterface)
	{
		MenuInterface->Leave();
	}
}