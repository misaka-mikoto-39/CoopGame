// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerupActor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APowerupActor::APowerupActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light Comp"));
	PointLightComp->SetAttenuationRadius(200.0f);
	PointLightComp->CastShadows = false;
	PointLightComp->SetupAttachment(RootComponent);
	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Comp"));
	IsPowerupActive = false;
	PowerupInterval = 0.0f;
	TotalNumOfTicks = 0;
	TickProcessed = 0;
	SetReplicates(true);
}

void APowerupActor::OnTickPowerup()
{
	TickProcessed++;
	OnPowerupTicked();
	if (TickProcessed >= TotalNumOfTicks)
	{
		OnExpired();
		IsPowerupActive = false;
		// Manually call in sv side
		OnRep_PowerupActive();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void APowerupActor::ActivatePowerup(AActor* ActivateFor)
{
	OnActivated(ActivateFor);
	IsPowerupActive = true;
	// Manually call in sv side
	OnRep_PowerupActive();
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void APowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(IsPowerupActive);
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APowerupActor, IsPowerupActive);
}