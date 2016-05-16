// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "CSUE.h"
#include "CSUECharacter.h"
#include "CSUEProjectile.h"
#include "Animation/AnimInstance.h"
#include "CSUETerrorist.h"
#include "CSUECounterTerrorist.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACSUECharacter

ACSUECharacter::ACSUECharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
    
    /*
	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
     */
    

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
    
    
    bShooting = false;
    charHealth = 100;
}

void ACSUECharacter::BeginPlay(){
    Super::BeginPlay();
    
	
    if(this->RifleClass){
        UWorld *World = GetWorld();
        if(World){
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            FRotator Rotation(0.f,0.f,-90.f);
            //myWeapon = new AWeapon(1.f,99999.f);
            myRifle = World->SpawnActor<ACSUERifle>(this->RifleClass,FVector::ZeroVector,Rotation,SpawnParams);
            if(myRifle){
                //UE_LOG(LogTemp,Warning,TEXT("ATTATCHED WEAPON"));
                myRifle->WeaponMesh->AttachTo(Mesh1P,TEXT("GripPoint"),EAttachLocation::SnapToTargetIncludingScale,true);
                myRifle->myPawn = this;
            }
        }
    }
	
	if (this->ShotgunClass) {
		UWorld *World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			FRotator Rotation(0.f, 0.f, -90.f);
			//myWeapon = new AWeapon(1.f,99999.f);
			myShotgun = World->SpawnActor<ACSUEShotgun>(this->ShotgunClass, FVector::ZeroVector, Rotation, SpawnParams);
			if (myShotgun) {
				//UE_LOG(LogTemp,Warning,TEXT("ATTATCHED WEAPON"));
				myShotgun->WeaponMesh->AttachTo(Mesh1P, TEXT("weaponStash"), EAttachLocation::SnapToTargetIncludingScale, true);
				myShotgun->myPawn = this;
			}
		}
	}

    
    
    
    
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACSUECharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACSUECharacter::TouchStarted);
	if( EnableTouchscreenMovement(InputComponent) == false )
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &ACSUECharacter::OnFire);
        InputComponent->BindAction("Fire",IE_Released,this,&ACSUECharacter::OnStopFire);
	}
	
	InputComponent->BindAxis("MoveForward", this, &ACSUECharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACSUECharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ACSUECharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ACSUECharacter::LookUpAtRate);

	//switch weapons
	InputComponent->BindAction("switch", IE_Pressed, this, &ACSUECharacter::switchWeapon);
}

void ACSUECharacter::switchWeapon() {
	if (currentWeapon == FString(TEXT("rifle"))) {
		myShotgun->WeaponMesh->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
		myRifle->WeaponMesh->AttachTo(Mesh1P, TEXT("weaponStash"), EAttachLocation::SnapToTargetIncludingScale, true);
		currentWeapon = FString(TEXT("shotgun"));
	}
	else {
		myShotgun->WeaponMesh->AttachTo(Mesh1P, TEXT("weaponStash"), EAttachLocation::SnapToTargetIncludingScale, true);
		myRifle->WeaponMesh->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
		currentWeapon = FString(TEXT("rifle"));


	}


}
void ACSUECharacter::OnFire()
{
    bShooting = true;
    if(myShotgun &&bShooting && currentWeapon == FString(TEXT("shotgun"))) {
        myShotgun->OnStartFire();
		//myShotgun->WeaponTrace();
    }
	else if (myRifle &&bShooting) {
		myRifle->OnStartFire();
	}
    /*
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<ACSUEProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
     */

}

void ACSUECharacter::OnStopFire(){
    bShooting = false;
	if (myShotgun &&!bShooting && currentWeapon == FString(TEXT("shotgun"))) {
		myShotgun->OnStopFire();
		//myShotgun->WeaponTrace();
	}
	else if (myRifle) {
		myRifle->OnStopFire();
	}
}

void ACSUECharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if( TouchItem.bIsPressed == true )
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ACSUECharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if( ( FingerIndex == TouchItem.FingerIndex ) && (TouchItem.bMoved == false) )
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ACSUECharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && ( TouchItem.FingerIndex==FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D( MoveDelta.X, MoveDelta.Y) / ScreenSize;									
					if (ScaledDelta.X != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (ScaledDelta.Y != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y* BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ACSUECharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACSUECharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACSUECharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACSUECharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ACSUECharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if(FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch )
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACSUECharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ACSUECharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ACSUECharacter::TouchUpdate);
	}
	return bResult;
}

FString ACSUECharacter::getEnemyTeam(){
    //if character is on CT, enemy is T
    UE_LOG(LogTemp,Warning,TEXT(" GET ENEMY TEAM +   %s"),*myTeam);

    if(myTeam == FString(TEXT("CT")))
        return FString(TEXT("T"));
    
    //if character is on T, enemy is CT
    else
        return FString(TEXT("CT"));

    
}

int32 ACSUECharacter::getHealth() const{
    return charHealth;
}
