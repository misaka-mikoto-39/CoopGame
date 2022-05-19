// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGameInstance.h"


UCoopGameInstance::UCoopGameInstance(const FObjectInitializer& ObjectIniyializer)
{}

void UCoopGameInstance::Init()
{}

void UCoopGameInstance::Host()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Hosting Server"));
	}
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Map/NewMap?listen");
	}
}

void UCoopGameInstance::Join(const FString& Address)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Joining ") + Address);
	}
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}
