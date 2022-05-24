// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
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
		UButton* JoinMenuButton;
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
		UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
		UPanelWidget* ServerList;
protected:
	IMenuInterface* MenuInterface;
	TOptional<uint32> SelectedIndex;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UFUNCTION()
		void OnClick_HostButton();
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
	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);
};
