// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "BeamEnd";
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		FVector ShootDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 1000);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		FHitResult Hit;

		FVector TracerEndPoint = TraceEnd;
		// play muzzle flash
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, MuzzleSocketName);
		}
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShootDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
			}
			//play hit effect
			if (HitEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
			TracerEndPoint = Hit.ImpactPoint;
		}

		if (TracerEffect)
		{
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if (TracerComp)
			{
				TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
			}
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}