// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class APowerupActor;

UCLASS()
class COOPGAME_API APickupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void ReSpawn();

	APowerupActor* PowerupInstance;
	FTimerHandle TimerHandle_RespawnPowerup;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
		TSubclassOf<APowerupActor> PowerupClass;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
		float CooldownDuration;

public:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
