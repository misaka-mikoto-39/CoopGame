// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameMode.h"
#include "ShooterLobbyGameMode.generated.h"

/**
 *
 */
UCLASS()
class COOPGAME_API AShooterLobbyGameMode : public AShooterGameMode
{
	GENERATED_BODY()
public:
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;
protected:
	FTimerHandle TimerHandle_StartGame;
private:
	UFUNCTION()
		void StartGame();
	int32 NumOfPlayers = 0;
};
