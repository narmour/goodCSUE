// Fill out your copyright notice in the Description page of Project Settings.

#include "CSUE.h"
#include "CSUEBomb.h"
#include "CSUETerrorist.h"
#include "CSUECounterTerrorist.h"
#include "CSUECharacter.h"
#include "CSUEGameMode.h"


// Sets default values
ACSUEBomb::ACSUEBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bomb"));
	RootComponent = bombMesh;

}

/*
bool ACSUEBomb::isPlanted() {
	return planted;
}
*/
// Called when the game starts or when spawned
void ACSUEBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSUEBomb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bombMesh)
		checkOverlap();

}


void ACSUEBomb::bombExplode() {
	UE_LOG(LogTemp, Warning, TEXT("EXPLODE"));
	if(explosionFX)
		UGameplayStatics::SpawnEmitterAtLocation(this, explosionFX, GetActorLocation());
	
	auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
	if (myGM) {
		myGM->endRound(FString(TEXT("t")));
	}

	GetWorldTimerManager().ClearTimer(bombTimer);
	planted = false;
	



}

void ACSUEBomb::bombDefused() {
	UE_LOG(LogTemp, Warning, TEXT("DEFUSED"));
	auto myGM = (ACSUEGameMode*)GetWorld()->GetAuthGameMode();
	if (myGM) {
		myGM->endRound(FString(TEXT("ct")));
		GetWorldTimerManager().ClearTimer(defuseTimer);

	}

}

void ACSUEBomb::checkOverlap() {
	TArray<AActor*> nearbyT;
	//UE_LOG(LogTemp, Warning, TEXT("FOUND ACTOR"));
	
	bombMesh->GetOverlappingActors(nearbyT);

	for (int32 i = 0; i < nearbyT.Num(); i++) {
		//UE_LOG(LogTemp, Warning, TEXT("FOUND ACTOR"));

		auto terrorist = Cast<ACSUETerrorist>(nearbyT[i]);
		auto ct = Cast<ACSUECounterTerrorist>(nearbyT[i]);
		auto player = Cast<ACSUECharacter>(nearbyT[i]);
		if (terrorist) {
			//UE_LOG(LogTemp, Warning, TEXT("FOUND TERRORIST"));
			//start bomb timer
			if (!planted) {
				GetWorldTimerManager().SetTimer(bombTimer, this, &ACSUEBomb::bombExplode, 20.f, false);
				planted = true;
				UE_LOG(LogTemp, Warning, TEXT("bomb planted"));

			}

		}
		//player plant
		if (player && player->getEnemyTeam() == FString(TEXT("ct"))) {
			//UE_LOG(LogTemp, Warning, TEXT("FOUND TERRORIST"));
			if (!planted) {
				GetWorldTimerManager().SetTimer(bombTimer, this, &ACSUEBomb::bombExplode, 10.f, false);
				planted = true;
				UE_LOG(LogTemp, Warning, TEXT("bomb planted"));

			}


		}
		//player defuse
		if (player && player->getEnemyTeam() == FString(TEXT("t"))) {
			UE_LOG(LogTemp, Warning, TEXT("player defuse"));
			if (planted && !GetWorldTimerManager().IsTimerActive(defuseTimer)) {
				GetWorldTimerManager().SetTimer(defuseTimer, this, &ACSUEBomb::bombDefused, 3.f, false);

			}
		}
		//ai defuse
		else if (ct) {
			//defuse shit

			//timer not started yet?
			if (planted && !GetWorldTimerManager().IsTimerActive(defuseTimer)) {
				GetWorldTimerManager().SetTimer(defuseTimer, this, &ACSUEBomb::bombDefused, 3.f, false);

				
			}
			
		}
		//if no is on the bomb, dont defuse
		else {
			//if defuse timer is active, turn it off
			if (GetWorldTimerManager().IsTimerActive(defuseTimer)) {
				GetWorldTimerManager().ClearTimer(defuseTimer);
			}

		}
	

	}

}

