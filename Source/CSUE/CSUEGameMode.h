// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "CSUEGameManager.h"
#include "CSUEBomb.h"
#include "CSUEGameMode.generated.h"

UCLASS(minimalapi)
class ACSUEGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACSUEGameMode();

	//GameManager will call these functions
    UFUNCTION(BlueprintCallable, Category = "round")
	void startRound();
	void endRound(FString winningTeam);

	void endGame(FString winningTeam);
    
    virtual void BeginPlay() override;
    
    ACSUEGameManager *getManager(){return myManager;};

	//HUD bp functions
	UFUNCTION(BlueprintCallable,Category = "HUD")
	int32 getTWins() { return tWins; };

	UFUNCTION(BlueprintCallable,Category = "HUD")

	int32 getCTWins() { return ctWins; };
	UFUNCTION(BlueprintCallable, Category = "HUD")
		FTimerHandle getTimer() { return roundTimer; };
	UFUNCTION(BlueprintCallable,Category = "HUD")
		int32 getTime() { return time; };



	void decrementTimer() { time -= 1; if (time < 1) endRound(FString(TEXT("ct"))); };
	UFUNCTION(BlueprintCallable, Category = "bomb")
	ACSUEBomb *getABomb() { return myABomb; };
	UFUNCTION(BlueprintCallable, Category = "bomb")
	ACSUEBomb *getBBomb() { return myBBomb; };
protected:
    UPROPERTY(EditAnywhere)
    
    TSubclassOf<ACSUEGameManager> managerClass;
	TSubclassOf<ACSUEBomb> bombClass;
    
    ACSUEGameManager *myManager;
	ACSUEBomb *myABomb;
	ACSUEBomb *myBBomb;
    
    int32 totalRoundsPlayed = 0;
    int32 tWins = 0;
    int32 ctWins =0;
	int32 time = 180;
	FTimerHandle roundTimer;
    
    //widget class for hud
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf<UUserWidget> HUDWidgetClass;
    
    //instance of hud
    UPROPERTY()
    UUserWidget *CurrentWidget;


    
    
};



