// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackerBot.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CoopGame/Components/HealthComponent.h"
#include "CoopGame/Character/ShooterCharacter.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ATrackerBot::ATrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ATrackerBot::HandleOnHealthChanged);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	bCanAffectNavigationGeneration = false;
	IsUseVelocityChange = false;
	MovementForce = 750.0f;
	RequiredDistanceToTarget = 100.0f;
	ExplosionBaseDamage = 50.0f;
	ExplosionRadius = 200.0f;
	IsExploded = false;
	IsStartedSelfDestruct = false;
	SelfDamage = 20.0f;
	SelfDamageRate = 0.25f;
}

// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}
}

FVector ATrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	if (NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}
	return GetActorLocation();
}

void ATrackerBot::SelfDestruct()
{
	if (IsExploded)
	{
		return;
	}
	IsExploded = true;

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
	if (ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	}
	MeshComp->SetVisibility(false, true);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this, ExplosionBaseDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoreActors, this, GetInstigatorController(), true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);

		SetLifeSpan(2.0f);
	}
}

void ATrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, SelfDamage, GetInstigatorController(), this, nullptr);
}

void ATrackerBot::HandleOnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}
	if (Health <= 0)
	{
		SelfDestruct();
	}
}

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority && !IsExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
		}
		else
		{
			//keep moving to next target
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;

			MeshComp->AddForce(ForceDirection, NAME_None, IsUseVelocityChange);
			//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Cyan, false, 0.0f, 0, 1.0f);
		}
		//DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Cyan, false, 0.0f, 1.0f);
	}
}

void ATrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsStartedSelfDestruct || IsExploded)
	{
		return;
	}
	AShooterCharacter* PlayerPawn = Cast<AShooterCharacter>(OtherActor);
	if (PlayerPawn)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ATrackerBot::DamageSelf, SelfDamageRate, true, 0.0f);
		}
		IsStartedSelfDestruct = true;
		if (SelfDestructSound)
		{
			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}