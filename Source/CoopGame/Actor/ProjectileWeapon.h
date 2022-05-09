// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 *
 */
UCLASS()
class COOPGAME_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> ProjectileClass;
	virtual void Fire() override;
public:
};
