// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterLobbyGameMode.h"
#include "CoopGameInstance.h"

void AShooterLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NumOfPlayers++;
	if (NumOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_StartGame, this, &AShooterLobbyGameMode::StartGame, 10.0f);
	}
}

void AShooterLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumOfPlayers--;
}

void AShooterLobbyGameMode::StartGame()
{
	UCoopGameInstance* GI = GetGameInstance<UCoopGameInstance>();
	UWorld* World = GetWorld();
	if (World && GI)
	{
		bUseSeamlessTravel = true;
		GI->StartSession();
		World->ServerTravel("/Game/Map/NewMap?listen");
	}
}