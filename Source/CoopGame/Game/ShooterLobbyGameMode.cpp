// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterLobbyGameMode.h"


void AShooterLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NumOfPlayers++;
	if (NumOfPlayers >= 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel("/Game/Map/NewMap?listen");
		}
	}
}

void AShooterLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumOfPlayers--;
}
