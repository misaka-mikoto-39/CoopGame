// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "ServerRowWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectIniyializer) : UUserWidget(ObjectIniyializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/MainMenu/WBP_ServerRowWidget"));
	if (ServerRowBPClass.Class != nullptr)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

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
	bIsFocusable = true;
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

void UMainMenuWidget::UpdateChildren()
{
	if (ServerList)
	{
		for (int i = 0; i < ServerList->GetChildrenCount(); i++)
		{
			UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
			if (Row)
			{
				Row->IsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
			}
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
		if (JoinMenu && MenuInterface)
		{
			MenuInterface->RefreshServerList();
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
		if (SelectedIndex.IsSet())
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
			MenuInterface->Join(SelectedIndex.GetValue());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected Index not set"))
		}
	}
}

void UMainMenuWidget::SetServerList(TArray<FServerData> ServerDatas)
{
	UWorld* World = this->GetWorld();
	if (World && ServerList)
	{
		ServerList->ClearChildren();
		for (int i = 0; i < ServerDatas.Num(); i++)
		{
			if (ServerRowClass)
			{
				UServerRowWidget* Row = CreateWidget<UServerRowWidget>(World, ServerRowClass);
				if (Row)
				{
					Row->SetServerData(FText::FromString(ServerDatas[i].ServerName), FText::FromString(ServerDatas[i].HostUserName), FText::FromString(FString::FromInt(ServerDatas[i].CurrentPlayers) + "/" + FString::FromInt(ServerDatas[i].MaxPlayers)));
					Row->Setup(this, i);
					ServerList->AddChild(Row);
				}
			}
		}
	}
}

void UMainMenuWidget::SelectIndex(int32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}