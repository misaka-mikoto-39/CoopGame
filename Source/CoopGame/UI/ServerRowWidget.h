// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class COOPGAME_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ServerName;
public:
	void SetServerText(FText Text);
};
