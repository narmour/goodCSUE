// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CSUEAICharacter.h"
#include "CSUECounterTerrorist.generated.h"

/**
 * 
 */
UCLASS()
class CSUE_API ACSUECounterTerrorist : public ACSUEAICharacter
{
	GENERATED_BODY()
	
public:
    //constructor
    //ACSUECounterTerrorist();
    
    virtual FString getEnemyTeam() override;
    
    virtual void Tick( float DeltaSeconds ) override;

    
private:
    FString enemyTeam = FString(TEXT("T"));
	
};
