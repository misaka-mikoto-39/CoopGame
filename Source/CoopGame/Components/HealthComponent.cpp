// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "CoopGame/Character/ShooterCharacter.h"
#include "CoopGame/GameModes/ShooterGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100;
	IsDead = false;
	SetIsReplicated(true);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleOnTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
}

void UHealthComponent::HandleOnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (Health <= 0.0f)
	{
		IsDead = true;
		AShooterGameMode* GM = GetWorld()->GetAuthGameMode<AShooterGameMode>();
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}
void UHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = OldHealth - Health;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0.0f || Health <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health + Amount, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, -Amount, nullptr, nullptr, nullptr);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}