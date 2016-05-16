
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CSUEAICharacter.h"
#include "CSUEAIEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CSUE_API ACSUEAIEnemyCharacter : public ACSUEAICharacter
{
	GENERATED_BODY()
	
public:
    
    virtual FString getEnemyTeam() override;
    
private:
    FString enemyTeam = FString(TEXT("T"));

	
	
};
