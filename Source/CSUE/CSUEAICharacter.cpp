// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUEGameMode.h"
#include "CSUEAICharacter.h"


// Sets default values
ACSUEAICharacter::ACSUEAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myHealth = 100.f;


}

void ACSUEAICharacter::destroySelf() {
	myWeapon->OnStopFire();
	myWeapon->Destroy();
	Destroy();
}



// Called when the game starts or when spawned
void ACSUEAICharacter::BeginPlay()
{
	Super::BeginPlay();
    if(this->weaponType){
        UWorld *World = GetWorld();
        if(World){
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            FRotator Rotation(0.f,0.f,-90.f);
            //myWeapon = new AWeapon(1.f,99999.f);
            myWeapon = World->SpawnActor<AWeapon>(this->weaponType,FVector::ZeroVector,Rotation,SpawnParams);
            if(myWeapon){
                //UE_LOG(LogTemp,Warning,TEXT("ATTATCHED WEAPON"));
                myWeapon->WeaponMesh->AttachTo(this->GetMesh(),TEXT("WeaponPoint"),EAttachLocation::SnapToTargetIncludingScale,true);
                myWeapon->SetActorRelativeRotation(Rotation);
                myWeapon->myPawn = this;
            }
        }
    }

	
}

// Called every frame
void ACSUEAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
    
    

}

// Called to bind functionality to input
void ACSUEAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ACSUEAICharacter::takeDamage(float damage) {
	myHealth -= damage;
	UE_LOG(LogTemp, Warning, TEXT("TOOK A HIT,   %f"), myHealth);
	//if we go negative reset back to 0
	if (myHealth < 0)
		myHealth = 0;
}

void ACSUEAICharacter::OnStartFire()
{
	myWeapon->OnStartFire();
}

void ACSUEAICharacter::OnStopFire()
{
	myWeapon->OnStopFire();
}