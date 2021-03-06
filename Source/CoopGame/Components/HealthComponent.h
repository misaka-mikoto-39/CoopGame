// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup = (COOP), meta = (BlueprintSpawnableComponent))
class COOPGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:

	bool IsDead;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "HealthComponent")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
		void HandleOnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
		void OnRep_Health(float OldHealth);

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "Events")
		FOnHealthChangedSignature OnHealthChanged;

public:
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		void Heal(float Amount);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HealthComponent")
		uint8 TeamNum;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
		static bool IsFriendly(AActor* ActorA, AActor* ActorB);
};
