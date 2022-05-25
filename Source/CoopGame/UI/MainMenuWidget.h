// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

public:
	FString ServerName;
	int32 CurrentPlayers;
	int32 MaxPlayers;
	FString HostUserName;
};

/**
 *
 */
UCLASS()
class COOPGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	TSubclassOf<UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
		UButton* HostMenuButton;
	UPROPERTY(meta = (BindWidget))
		UButton* JoinMenuButton;
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton; 
	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton2;
	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
		UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
		UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* HostName;

	void UpdateChildren();
protected:
	IMenuInterface* MenuInterface;
	TOptional<int32> SelectedIndex;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UFUNCTION()
		void OnClick_HostButton();
	UFUNCTION()
		void OnClick_HostMenuButton();
	UFUNCTION()
		void OnClick_JoinMenuButton();
	UFUNCTION()
		void OnClick_QuitButton();
	UFUNCTION()
		void OnClick_CancelButton();
	UFUNCTION()
		void OnClick_JoinButton();
public:
	UMainMenuWidget(const FObjectInitializer& ObjectIniyializer);
	virtual bool Initialize() override;
	void SetMenuInterface(IMenuInterface* NewMenuInterface);
	void Setup();
	void SetServerList(TArray<FServerData> ServerDatas);
	void SelectIndex(int32 Index);
};
