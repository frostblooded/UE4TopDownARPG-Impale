// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TopDownARPG.h"
#include "Spike.generated.h"

UCLASS()
class TOPDOWNARPG_API ASpike : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpike();

protected:
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int32 RemainingSpikes = 6;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float LifeSpan = 0.2;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FVector GetSpawnLocation();
	void SpawnNextSpike();
};
