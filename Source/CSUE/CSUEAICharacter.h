// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "CSUEAICharacter.generated.h"
//make this an abstract class
UCLASS(abstract)
class CSUE_API ACSUEAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSUEAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	void takeDamage(float damage);
    
    //virtual to get enemyType for Weapon class
    virtual FString getEnemyTeam() PURE_VIRTUAL(ACSUEAICharacter::getEnemyTeam,return TEXT("x"););

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnStartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnStopFire();

	//called when health is 0
	void destroySelf();
protected:
	//character health
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float myHealth;
    
    UPROPERTY(EditAnywhere,Category = "Weapon")
    TSubclassOf<AWeapon>weaponType;
    
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Weapon")
    AWeapon *myWeapon;
    
   

	
	
};
