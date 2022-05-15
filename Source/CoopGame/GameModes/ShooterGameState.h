// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaveState_None,
	WaitingToStart,
	WaveInProgess,
	WaitingToComplete,
	WaveComplete,
	GameOver,

	WaveState_Count
};

/**
 *
 */
UCLASS()
class COOPGAME_API AShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
		void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnWaveStateChanged(EWaveState NewState, EWaveState OldState);

	UPROPERTY(ReplicatedUsing = OnRep_WaveState, BlueprintReadOnly, Category = "GameState")
		EWaveState WaveState;
public:
	AShooterGameState();
	EWaveState GetWaveState();
	void SetWaveState(EWaveState NewState);
};
