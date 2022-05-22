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

protected:
	IMenuInterface* MenuInterface;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* JoinMenuButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* QuitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* JoinButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidget* JoinMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* IPAddressText;

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
	virtual bool Initialize() override;
	void SetMenuInterface(IMenuInterface* NewMenuInterface);
	void Setup();
};
