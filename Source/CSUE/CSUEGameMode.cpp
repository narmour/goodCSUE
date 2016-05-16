// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "CSUE.h"
#include "CSUEGameMode.h"
#include "CSUEHUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "CSUECharacter.h"

ACSUEGameMode::ACSUEGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACSUEHUD::StaticClass();
    
    static ConstructorHelpers::FObjectFinder<UBlueprint> managerBlueprint(TEXT("/Game/FirstPersonCPP/Blueprints/CSUEGameManager_BP"));
    if(managerBlueprint.Object){
		UE_LOG(LogTemp, Warning, TEXT("managerBLUEPRINT SET"));

        managerClass = (UClass*)managerBlueprint.Object->GeneratedClass;
    }

	static ConstructorHelpers::FObjectFinder<UBlueprint> bombBlueprint(TEXT("/Game/FirstPersonCPP/Blueprints/CSUEBomb_BP"));
	if (bombBlueprint.Object) {
		UE_LOG(LogTemp, Warning, TEXT("bombBLUEPRINT SET"));

		bombClass = (UClass*)bombBlueprint.Object->GeneratedClass;
	}
    
    

}

void ACSUEGameMode::BeginPlay(){
    UE_LOG(LogTemp, Warning, TEXT("HI FROM GAMEMODE BEGINPLAY"));
    
    if(HUDWidgetClass){
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
        if(CurrentWidget){
            CurrentWidget->AddToViewport();
        }
    }
    
    if(managerClass){
        auto myWorld = GetWorld();
        if(myWorld)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            myManager = myWorld->SpawnActor<ACSUEGameManager>(managerClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParams);
			UE_LOG(LogTemp, Warning, TEXT("MANAGER SPAWNED SPAWNED"));

        
        }

	}
	if (bombClass) {
		auto myWorld = GetWorld();
		UE_LOG(LogTemp, Warning, TEXT("BOMBS SPAWNED"));

		if (myWorld) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			//hardcoded spawnpoints
			FVector aSpawn(-869.f, 245.f, 156.f);
			FVector bSpawn(-9400.0f, 245.f, 79.f);
			//spawn both bombs
			myABomb = myWorld->SpawnActor<ACSUEBomb>(bombClass, aSpawn, FRotator::ZeroRotator, SpawnParams);
			myBBomb = myWorld->SpawnActor<ACSUEBomb>(bombClass, bSpawn, FRotator::ZeroRotator, SpawnParams);
			UE_LOG(LogTemp, Warning, TEXT("BOMBS SPAWNED"));
		}
    }
    
    //startRound();
    

}


void ACSUEGameMode::endGame(FString winningTeam) {
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
	//reset rounds
	tWins = 0;
	ctWins = 0;

	//do more post-game stuff here
}
void ACSUEGameMode::startRound(){
    totalRoundsPlayed +=1;
    myManager->initTeams();
	//float time = 180.0f;
	GetWorldTimerManager().SetTimer(roundTimer,this,&ACSUEGameMode::decrementTimer,1.f,true);
    
   // myManager->
    
}

void ACSUEGameMode::endRound(FString winningTeam){
    if(winningTeam == FString(TEXT("t")))
        tWins+=1;
    else
        ctWins+=1;
	//reset round time
	GetWorldTimerManager().ClearTimer(roundTimer);
	time = 180;
	//start new round if game is not over
    UE_LOG(LogTemp,Warning,TEXT("ROUND OVER"));
	if (tWins == 5 || ctWins == 5)
		endGame(winningTeam);
	else
		startRound();

}
