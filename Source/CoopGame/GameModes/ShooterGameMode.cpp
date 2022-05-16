// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGameMode.h"
#include "CoopGame/Components/HealthComponent.h"
#include "ShooterGameState.h"
#include "ShooterPlayerState.h"

AShooterGameMode::AShooterGameMode()
{
	GameStateClass = AShooterGameState::StaticClass();
	PlayerStateClass = AShooterPlayerState::StaticClass();
	TimeBetweenWave = 5.0f;
	// tick 1 time per second
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AShooterGameMode::StartWave()
{
	WaveCount++;
	NumOfBotToSpawn = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AShooterGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
	SetWaveState(EWaveState::WaveInProgess);
}

void AShooterGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	SetWaveState(EWaveState::WaitingToComplete);
}

void AShooterGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AShooterGameMode::StartWave, TimeBetweenWave, false);
	RespawnDeadPlayer();
	SetWaveState(EWaveState::WaitingToStart);
}

void AShooterGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NumOfBotToSpawn--;
	if (NumOfBotToSpawn <= 0)
	{
		EndWave();
	}
}

void AShooterGameMode::CheckWaveState()
{
	bool IsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (NumOfBotToSpawn > 0 || IsPreparingForWave)
	{
		return;
	}
	bool IsAnyBotAlive = false;
	for (auto it = GetWorld()->GetPawnIterator(); it; ++it)
	{
		APawn* TestPawn = it->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		UHealthComponent* TestPawnHealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0.0f)
		{
			IsAnyBotAlive = true;
			break;
		}
	}
	if (!IsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void AShooterGameMode::CheckAnyPlayerAlive()
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController* PC = it->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	// no player alive
	GameOver();
}

void AShooterGameMode::GameOver()
{
	EndWave();
	SetWaveState(EWaveState::GameOver);
}

EWaveState AShooterGameMode::GetWaveState()
{
	AShooterGameState* GS = GetGameState<AShooterGameState>();
	if (ensureAlways(GS))
	{
		return GS->GetWaveState();
	}
	return EWaveState::WaveState_None;
}

void AShooterGameMode::SetWaveState(EWaveState NewState)
{
	AShooterGameState* GS = GetGameState<AShooterGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void AShooterGameMode::RespawnDeadPlayer()
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController* PC = it->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void AShooterGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
}

void AShooterGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckWaveState();
	CheckAnyPlayerAlive();
}