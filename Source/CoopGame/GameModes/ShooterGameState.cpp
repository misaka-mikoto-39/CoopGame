// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGameState.h"
#include "Net/UnrealNetwork.h"

AShooterGameState::AShooterGameState()
{
	WaveState = EWaveState::WaveState_None;
}

EWaveState AShooterGameState::GetWaveState()
{
	return WaveState;
}

void AShooterGameState::SetWaveState(EWaveState NewState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		EWaveState OldState = WaveState;
		WaveState = NewState;
		// call on server
		OnRep_WaveState(OldState);
	}
}

void AShooterGameState::OnRep_WaveState(EWaveState OldState)
{
	OnWaveStateChanged(WaveState, OldState);
}

void AShooterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterGameState, WaveState);
}