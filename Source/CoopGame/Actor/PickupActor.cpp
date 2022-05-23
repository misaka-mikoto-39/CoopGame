// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PowerupActor.h"
#include "CoopGame/Character/ShooterCharacter.h"

// Sets default values
APickupActor::APickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Comp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);
	CooldownDuration = 10.0f;
	bReplicates = true;
}
// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		ReSpawn();
	}
}

void APickupActor::ReSpawn()
{
	if (PowerupClass == nullptr)
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerupInstance = GetWorld()->SpawnActor<APowerupActor>(PowerupClass, GetTransform(), SpawnParams);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	/*AShooterCharacter* SC = Cast<AShooterCharacter>(OtherActor);
	if (SC && SC->IsPlayerControlled())
	{*/
	if (GetLocalRole() == ROLE_Authority && PowerupInstance)
	{
		PowerupInstance->ActivatePowerup(OtherActor);
		PowerupInstance = nullptr;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnPowerup, this, &APickupActor::ReSpawn, CooldownDuration);
	}
	//}
}