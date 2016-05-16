// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUEShotgun.h"
#include "CSUEAICharacter.h"
#include "CSUECharacter.h"



ACSUEShotgun::ACSUEShotgun():AWeapon(9999.f,20.f,2.f,7.f) {
	
}


void ACSUEShotgun::OnStartFire()
{
	if (!bShooting && currentAmmo >0) {
		//UE_LOG(LogTemp, Warning, TEXT("ON START"));

		bShooting = true;
		ShootAC = PlayWeaponSound(FireLoopSound);
		muzzlePSC = muzzleFlash(MuzzleFX);
		WeaponTrace();
		currentAmmo -= 1;
		GetWorldTimerManager().SetTimer(shootingTimer, this, &AWeapon::shooting, weaponFireRate, false);
	}
	//if you outta bullets and havent started to reload yet
	else if (!(currentAmmo > 0) && !(GetWorldTimerManager().IsTimerActive(reloadTimer))) {
		GetWorldTimerManager().SetTimer(reloadTimer, this, &AWeapon::reload, 3.f, false);
	}
	

}



void ACSUEShotgun::WeaponTrace() {
	//cast pawn to player char and AI char
	auto myFPChar = Cast<ACSUECharacter>(myPawn);
	static FName MuzzleSocket = FName(TEXT("MuzzleFlashSocket"));

	FVector startPos = myPawn->GetActorLocation();
	FVector forward;
	//if its the player char, use camera forward, if its an AI, use actor forward
	if (myFPChar) {
		forward = myFPChar->GetFirstPersonCameraComponent()->GetForwardVector();
		//add camera height if its the player
		startPos.Z += 64;
	}
	//spray patterns?
	FVector endPos = startPos + (forward.GetSafeNormal() * weaponRange);


	//check if we blasted an enemy wit our weapon
	FCollisionQueryParams traceParams(FName(TEXT("WeaponTrace")), true, Instigator);
	traceParams.bTraceAsyncScene = true;
	traceParams.bReturnPhysicalMaterial = true;

	//check our fire ray against all objects with collision
	//shotgun spray 6 hits
	for (int i = 0; i < 6; i++) {
		UE_LOG(LogTemp, Warning, TEXT("SHOTGUN WEAPONTRACE"));

		FVector spread = endPos;
		FHitResult Hit(ForceInit);
		spread.X += FMath::FRandRange(-300, 300);
		spread.Y += FMath::FRandRange(-300, 300);
		spread.Z += FMath::FRandRange(-300, 300);

		GetWorld()->LineTraceSingleByObjectType(Hit, startPos, spread, FCollisionObjectQueryParams::AllObjects, traceParams);
		if (Hit.bBlockingHit) {
			//spawn hit effect particle
			if (Hit.GetActor()) {
				UGameplayStatics::SpawnEmitterAtLocation(Hit.GetActor(), HitFX, Hit.ImpactPoint);
				//if we hit an enemy, deal damage
				auto hitEnemy = Cast<ACSUEAICharacter>(Hit.GetActor());
				if (hitEnemy && enemyType != hitEnemy->getEnemyTeam()) {
					hitEnemy->takeDamage(weaponDamage);
				}

			}

		}
	}
	

}