// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackerBot.generated.h"

class UHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class COOPGAME_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	FVector GetNextPathPoint();
	void SelfDestruct();

	UFUNCTION()
		void DamageSelf();

	UFUNCTION()
		void HandleOnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector NextPathPoint;
	UMaterialInstanceDynamic* MatInst;
	bool IsExploded;
	bool IsStartedSelfDestruct;
	FTimerHandle TimerHandle_SelfDamage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		bool IsUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		float RequiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		float ExplosionBaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		float SelfDamageRate;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		float SelfDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "Tracker Bot")
		USoundCue* ExplodeSound;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
