// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUEGameMode.h"
#include "CSUECounterTerrorist.h"




void ACSUECounterTerrorist::Tick(float DeltaTime){
    if(myHealth ==0){
        auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
        if(myGM)
            myGM->getManager()->killCT();
        //myWeapon->Destroy();
        //Destroy();
		destroySelf();

    }
}

FString ACSUECounterTerrorist::getEnemyTeam(){
    return enemyTeam;
}