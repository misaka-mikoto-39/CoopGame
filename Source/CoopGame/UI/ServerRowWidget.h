// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

class UMainMenuWidget;
class UTextBlock;
class UButton;
/**
 *
 */
UCLASS()
class COOPGAME_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	int32 Index;

	UPROPERTY()
		UMainMenuWidget* Parent;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HostUserName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* NumOfPlayers;
	UPROPERTY(meta = (BindWidget))
		UButton* RowButton;

	UFUNCTION()
		void OnClick_RowButton();
public:
	UPROPERTY(BlueprintReadOnly)
		bool IsSelected = false;
	void SetServerData(FText InServerName, FText InHostUserName, FText InNumOfPlayers);
	void Setup(UMainMenuWidget* Parent, int32 Index);
};
