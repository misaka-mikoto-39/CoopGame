// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionClient.h"
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
	IOnlineSubsystem* OnlineSubsystem;
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> IngameMenuClass;
	IOnlineSessionPtr SessionInterface;
	IOnlineIdentityPtr IdentityInterface;
	UMainMenuWidget* MainMenu;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString DesiredServerName;

	//FOnSessionUserInviteAcceptedDelegate SessionInviteAcceptedDelegate;
	void CreateSession(FName InSessionName);
	void OnCreateSessionComplete(FName InSessionName, bool IsSuccess);
	void OnDestroySessionComplete(FName InSessionName, bool IsSuccess);
	void OnFindSessionComplete(bool IsSuccess);
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void OnSessionUserInviteAccepted(bool bWasSuccessful, int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& SearchResult);
	void OnLoginCompleted(int LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
public:
	UCoopGameInstance(const FObjectInitializer& ObjectIniyializer);
	virtual void Init() override;
	virtual void RefreshServerList() override;
	void StartSession();
	void Login();

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();
	UFUNCTION(BlueprintCallable)
		void LoadIngameMenu();
	UFUNCTION(Exec)
		virtual void Host(FString ServerName) override;
	UFUNCTION(Exec)
		virtual void Join(int32 Index) override;
	UFUNCTION(Exec)
		virtual void Leave() override;
};
