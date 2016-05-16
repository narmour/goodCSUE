// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUEGameMode.h"
#include "CSUEGameManager.h"


// Sets default values
ACSUEGameManager::ACSUEGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    

}

// Called when the game starts or when spawned
void ACSUEGameManager::BeginPlay()
{
	Super::BeginPlay();
    
    UE_LOG(LogTemp,Warning,TEXT("HI FROM BEGINPLAY"));
    //find spawn points in world and put them in respective arrays
    
    for(TActorIterator<ATargetPoint> targetItr(GetWorld());targetItr;++targetItr){
        auto tPoint = *targetItr;
        
        //if its a ct spawn point, put in ct array, else put in t array
        if(tPoint->GetName().Contains(FString(TEXT("Counter")))){
            ctSpawns.Add(tPoint);
            
        }
        else if (tPoint->GetName().Contains(FString(TEXT("Terrorist"))) ){
            tSpawns.Add(tPoint);
        }
    }
   // UE_LOG(LogTemp,Warning,TEXT("BEGINPLAY  T: %i    CT:%i "),tSpawns.Num(),ctSpawns.Num());
    

    auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
    //if(myGM)
        //myGM->startRound();
    //initTeams();
    
    
    
	
}

// Called every frame
void ACSUEGameManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACSUEGameManager::initTeams(){

	//clear arrays and delete all actors if any left
	for (int i = 0; i < tTeam.Num(); i++) {
		auto t = (ACSUETerrorist*)tTeam[i];
		if (t) 
			t->destroySelf();
	}
	for (int i = 0; i < ctTeam.Num(); i++) {
		auto ct = (ACSUECounterTerrorist*)ctTeam[i];
		if (ct)
			ct->destroySelf();
	}

	tTeam.Empty();
	ctTeam.Empty();








	//UE_LOG(LogTemp,Warning,TEXT(" T: %i    CT:%i "),tTeam.Num(),tTeam.Num());


    //if we set spawn points spawn 5 CT and 5 T
    UWorld* World = GetWorld();
    //UE_LOG(LogTemp,Warning,TEXT("TRIED TO SPAWN  T: %i    CT:%i "),tSpawns.Num(),ctSpawns.Num());
   
    if(tSpawns.Num() ==5 && ctSpawns.Num() == 5){

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            FRotator Rotation(0.f,180.f,0.f);
        for(int i =0;i<tSpawns.Num();i++){
            FVector spawnLoc = tSpawns[i]->GetActorLocation();
            //spawn terrorist team
            auto terrorist = World->SpawnActor<ACSUETerrorist>(tClass,spawnLoc,FRotator::ZeroRotator,SpawnParams);
			//Spawn controller
			terrorist->SpawnDefaultController();
            tTeam.Add(terrorist);
       
            spawnLoc = ctSpawns[i]->GetActorLocation();
            //spawn ct team
            auto counterTerrorist = World->SpawnActor<ACSUECounterTerrorist>(ctClass,spawnLoc,FRotator::ZeroRotator,SpawnParams);
			counterTerrorist->SpawnDefaultController();
            ctTeam.Add(counterTerrorist);
            
        }
        
        
        
    }
    ctAlive = ctTeam.Num();
    tAlive = tTeam.Num();
}

void ACSUEGameManager::killT(){
    tAlive-=1;
    UE_LOG(LogTemp,Warning,TEXT("KILLED T"));
    //if(tAlive)
    if(tAlive ==0){
        auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
        myGM->endRound(FString(TEXT("ct")));
    }
}

void ACSUEGameManager::killCT(){
    ctAlive-=1;
    UE_LOG(LogTemp,Warning,TEXT("KILLED CT"));
    if (ctAlive ==0){
        auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
        myGM->endRound(FString(TEXT("t")));
    }

}
