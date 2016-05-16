// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Weapon.generated.h"

UCLASS()

class CSUE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    AWeapon();
    //constructor for child classes
	AWeapon(float wR,float wD,float wFR,float wMA);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;
    
    UPROPERTY(EditDefaultsOnly,Category = "Sound")
    USoundCue *FireLoopSound;
    
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundCue *FireFinishSound;
    
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem *MuzzleFX;
    
    UPROPERTY(EditDefaultsOnly,Category = "Effects")
    UParticleSystem *HitFX;
    
    float weaponRange;
    float weaponDamage;
    float weaponFireRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	float maxAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	float currentAmmo;

	FTimerHandle reloadTimer;
	//bool bReloading;
    //Functions to shoot a bullet and check what it hits
    void OnStartFire();
    void OnStopFire();
    void WeaponTrace();
    FTimerHandle shootingTimer;
    //subclass to only apply damage to enemy type of myPawn
    FString enemyType;
	void shooting() { bShooting = false; OnStopFire(); };
	void reload() { currentAmmo = maxAmmo; GetWorldTimerManager().ClearTimer(reloadTimer); };
	bool bShooting = false;
    APawn *myPawn;
	
protected:
    //function to use audio/muzzle flash
    UAudioComponent *PlayWeaponSound(USoundCue *Sound);
    UParticleSystemComponent *muzzleFlash(UParticleSystem *particle);
    
    
    UPROPERTY(Transient)
    UAudioComponent *ShootAC;
    
    UPROPERTY(Transient)
    UParticleSystemComponent *muzzlePSC;

   
	
};
