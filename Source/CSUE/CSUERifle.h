// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "CSUERifle.generated.h"

/**
 * 
 */
UCLASS()
class CSUE_API ACSUERifle : public AWeapon
{
	GENERATED_BODY()
	
public:
	void OnStartFire();
	
	
};
