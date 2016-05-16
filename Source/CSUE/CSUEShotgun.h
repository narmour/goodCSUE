// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "CSUEShotgun.generated.h"

/**
 * 
 */
UCLASS()
class CSUE_API ACSUEShotgun : public AWeapon
{
	GENERATED_BODY()
public:
		ACSUEShotgun();
		void WeaponTrace();
		void OnStartFire();
	
	
	
};
