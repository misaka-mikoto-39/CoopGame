// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoopGame/UI/MenuInterface.h"
#include "CoopGameInstance.generated.h"

/**
 *
 */
UCLASS()
class COOPGAME_API UCoopGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
private:
	TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> IngameMenuClass;
public:
	UCoopGameInstance(const FObjectInitializer& ObjectIniyializer);
	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMenu();
	UFUNCTION(BlueprintCallable)
		void LoadIngameMenu();
	UFUNCTION(Exec)
		virtual void Host() override;
	UFUNCTION(Exec)
		virtual void Join(const FString& Address) override;
	UFUNCTION(Exec)
		virtual void Leave();
};
