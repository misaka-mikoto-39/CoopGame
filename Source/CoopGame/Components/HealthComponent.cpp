// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "CoopGame/Character/ShooterCharacter.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleOnTakeAnyDamage);
		//OnHealthChanged.AddDynamic(Cast<AShooterCharacter>(GetOwner()), &AShooterCharacter::OnHealthChanged);
	}
	Health = DefaultHealth;
}

void UHealthComponent::HandleOnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}