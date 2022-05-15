// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, Victim, AActor*, Killer, AController*, KillerController);
/**
 *
 */
UCLASS()
class COOPGAME_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	FTimerHandle TimerHandle_BotSpawner;
	FTimerHandle TimerHandle_NextWaveStart;
	int32 NumOfBotToSpawn;
	int32 WaveCount = 0;
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float TimeBetweenWave;
protected:
	void StartWave();
	void EndWave();
	void PrepareForNextWave();
	void SpawnBotTimerElapsed();
	void CheckWaveState();
	void CheckAnyPlayerAlive();
	void GameOver();
	EWaveState GetWaveState();
	void SetWaveState(EWaveState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewBot();

public:
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled OnActorKilled;
public:
	AShooterGameMode();
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
