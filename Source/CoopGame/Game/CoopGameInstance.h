// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "CoopGame/UI/MenuInterface.h"
#include "CoopGameInstance.generated.h"

namespace EOnJoinSessionCompleteResult
{
	enum Type;
}
class UMainMenuWidget;

/**
 *
 */
UCLASS()
class COOPGAME_API UCoopGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
private:
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> IngameMenuClass;
	IOnlineSessionPtr SessionInterface;
	UMainMenuWidget* MainMenu;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void CreateSession(FName SessionName = "My Session Game");
	void OnCreateSessionComplete(FName SessionName, bool IsSuccess);
	void OnDestroySessionComplete(FName SessionName, bool IsSuccess);
	void OnFindSessionComplete(bool IsSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
public:
	UCoopGameInstance(const FObjectInitializer& ObjectIniyializer);
	virtual void Init() override;
	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();
	UFUNCTION(BlueprintCallable)
		void LoadIngameMenu();
	UFUNCTION(Exec)
		virtual void Host() override;
	UFUNCTION(Exec)
		virtual void Join(uint32 Index) override;
	UFUNCTION(Exec)
		virtual void Leave() override;
};
