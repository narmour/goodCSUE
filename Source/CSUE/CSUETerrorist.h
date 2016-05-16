// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CSUEAICharacter.h"
#include "CSUETerrorist.generated.h"

/**
 * 
 */
UCLASS()
class CSUE_API ACSUETerrorist : public ACSUEAICharacter
{
	GENERATED_BODY()


public:
    
    virtual FString getEnemyTeam() override;
    virtual void Tick( float DeltaSeconds ) override;

    
private:
    FString enemyTeam = FString(TEXT("CT"));

	
	
	
	
};
