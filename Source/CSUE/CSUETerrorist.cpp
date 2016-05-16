// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUEGameMode.h"
#include "CSUETerrorist.h"




void ACSUETerrorist::Tick(float DeltaTime){
    if(myHealth ==0){
        auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
        if(myGM)
            myGM->getManager()->killT();
        //myWeapon->Destroy();
        //Destroy();
		destroySelf();
    }

}

FString ACSUETerrorist::getEnemyTeam(){
    return enemyTeam;
}