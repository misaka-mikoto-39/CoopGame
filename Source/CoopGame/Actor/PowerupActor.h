// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupActor.generated.h"

class URotatingMovementComponent;
class UPointLightComponent;

UCLASS()
class COOPGAME_API APowerupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerupActor();

protected:
	UFUNCTION()
		void OnTickPowerup();
	UFUNCTION()
		void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnPowerupStateChanged(bool NewState);

	FTimerHandle TimerHandle_PowerupTick;
	int32 TickProcessed;

	// Time between powerup ticks
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float PowerupInterval;

	// Total time apply the powerup effect
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 TotalNumOfTicks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		URotatingMovementComponent* RotatingMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPointLightComponent* PointLightComp;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
		bool IsPowerupActive;

public:

	void ActivatePowerup(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnActivated(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnPowerupTicked();
};
