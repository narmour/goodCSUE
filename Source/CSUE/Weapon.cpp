// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUECharacter.h"
#include "CSUEAICharacter.h"
#include "Weapon.h"
#include "CSUETerrorist.h"
#include "CSUECounterTerrorist.h"




//default construct
AWeapon::AWeapon(){
    PrimaryActorTick.bCanEverTick = true;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("base CLASS"));
    RootComponent = WeaponMesh;
    
    //init shooting audio loop
    ShootAC = nullptr;

    weaponRange = 9999.f;
    weaponDamage = 20.f;
    weaponFireRate = .05f;
	maxAmmo = 30.f;
	currentAmmo = 30.f;
}


// Sets values for child classes
AWeapon::AWeapon(float wR,float wD,float wFR,float wMA)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("base CLASS"));
    RootComponent = WeaponMesh;
    
    //init shooting audio loop
    ShootAC = nullptr;
    
    
    //weapon stats
    weaponRange = wR;
    weaponDamage = wD;
    weaponFireRate = wFR;
	maxAmmo = wMA;
	currentAmmo = wMA;
    
    
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
    auto playerChar = Cast<ACSUECharacter>(myPawn);
	auto aiChar = Cast<ACSUEAICharacter>(myPawn);

    //check if the weapon is equiped by player char, if not, get enemy type from AICHAR
    if(playerChar)
        enemyType =playerChar->getEnemyTeam();
    else if (aiChar){
        enemyType = aiChar->getEnemyTeam();
        //pure virtual works, doope
        UE_LOG(LogTemp,Warning,TEXT("%s"),*enemyType);
    }

	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound){
    UAudioComponent *AC = NULL;
    
    if (Sound) {
        AC = UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
    }
    return AC;
}

UParticleSystemComponent * AWeapon::muzzleFlash(UParticleSystem * particle)
{
    UParticleSystemComponent *flash = NULL;
    
    if (particle) {
        flash = UGameplayStatics::SpawnEmitterAttached(particle,RootComponent,TEXT("MuzzleFlashSocket"));
        
    }
    return flash;
}

void AWeapon::OnStartFire()
{
	if (!bShooting && currentAmmo >0) {
		UE_LOG(LogTemp, Warning, TEXT("ON START"));

		bShooting = true;
		ShootAC = PlayWeaponSound(FireLoopSound);
		muzzlePSC = muzzleFlash(MuzzleFX);
		WeaponTrace();
		GetWorldTimerManager().SetTimer(shootingTimer, this, &AWeapon::shooting, weaponFireRate, false);
		
	}
	//if you outta bullets and havent started to reload yet
	else if (!(currentAmmo > 0) && !(GetWorldTimerManager().IsTimerActive(reloadTimer))) {
		GetWorldTimerManager().SetTimer(reloadTimer, this, &AWeapon::reload, 3.f, false);
	}
}

void AWeapon::OnStopFire()
{
	bShooting = false;
    GetWorldTimerManager().ClearTimer(shootingTimer);
	if(ShootAC)
		ShootAC->Deactivate();
	if(muzzlePSC)
		muzzlePSC->DeactivateSystem();
}



void AWeapon::WeaponTrace(){
	//use a bullet
	currentAmmo -= 1;
	
	//if you outta bullets and havent started to reload yet
	if (currentAmmo <1) {
		GetWorldTimerManager().SetTimer(reloadTimer, this, &AWeapon::reload, 3.f, false);
		OnStopFire();
	}
	else {
		//cast pawn to player char and AI char
		auto myFPChar = Cast<ACSUECharacter>(myPawn);
		auto myAIChar = Cast<ACSUEAICharacter>(myPawn);
		static FName MuzzleSocket = FName(TEXT("MuzzleFlashSocket"));
		//bShooting = false;
		UE_LOG(LogTemp, Warning, TEXT("WEAPONTRACE"));



		FVector startPos = myPawn->GetActorLocation();
		FVector forward;
		//if its the player char, use camera forward, if its an AI, use actor forward
		if (myFPChar) {
			forward = myFPChar->GetFirstPersonCameraComponent()->GetForwardVector();
			//add camera height if its the player
			startPos.Z += 64;
		}
		else if (myAIChar)
		{
			forward = myAIChar->GetActorForwardVector();
		}
		//spray patterns?
		FVector endPos = startPos + (forward.GetSafeNormal() * weaponRange);


		//check if we blasted an enemy wit our weapon
		FCollisionQueryParams traceParams(FName(TEXT("WeaponTrace")), true, Instigator);
		traceParams.bTraceAsyncScene = true;
		traceParams.bReturnPhysicalMaterial = true;

		//check our fire ray against all objects with collision
		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(Hit, startPos, endPos, FCollisionObjectQueryParams::AllObjects, traceParams);
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

