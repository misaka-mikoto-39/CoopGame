// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "IngameMenuWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class COOPGAME_API UIngameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	IMenuInterface* MenuInterface;
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* QuitButton;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UFUNCTION()
		void OnClick_CancelButton();
	UFUNCTION()
		void OnClick_QuitButton();
public:
	virtual bool Initialize() override;
	void SetMenuInterface(IMenuInterface* NewMenuInterface);
	void Setup();
};
