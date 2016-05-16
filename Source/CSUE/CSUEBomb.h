// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CSUEBomb.generated.h"

UCLASS()
class CSUE_API ACSUEBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSUEBomb();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void checkOverlap();
	void bombDefused();
	void bombExplode();
	UFUNCTION(BlueprintCallable, Category = "bomb")
	bool isPlanted() {
		return planted;
	};


private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent *bombMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem *explosionFX;

	//function to play explosion effect
	UParticleSystemComponent *startExplosion(UParticleSystem *particle);


	FTimerHandle defuseTimer;
	FTimerHandle bombTimer;

	bool planted = false;








	
	
};
