// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUERifle.h"




void ACSUERifle::OnStartFire()
{
	
	if (currentAmmo >0) {
		//UE_LOG(LogTemp, Warning, TEXT("ON START"));

		
		ShootAC = PlayWeaponSound(FireLoopSound);
		muzzlePSC = muzzleFlash(MuzzleFX);
		//WeaponTrace();
		currentAmmo -= 1;
		GetWorldTimerManager().SetTimer(shootingTimer, this, &AWeapon::WeaponTrace, weaponFireRate, true);
	}
	//if you outta bullets and havent started to reload yet
	else if (!(currentAmmo > 0) && !(GetWorldTimerManager().IsTimerActive(reloadTimer))) {
		GetWorldTimerManager().SetTimer(reloadTimer, this, &AWeapon::reload, 3.f, false);
	}
	
}
