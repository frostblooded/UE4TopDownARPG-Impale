// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"

// Sets default values
ASpike::ASpike()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");

	if (IsValid(BoxComponent) == false) {
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::ASpike() - IsValid(BoxComponent) == false"));
	}

	BoxComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = BoxComponent;

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpike::OnOverlap);
}

void ASpike::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTopDownARPG, Error, TEXT("Overlapped with %s"), *GetNameSafe(Other));

	if (IsValid(Other) == false) {
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::OnOverlap() - IsValid(Other) == false"));
	}
}

void ASpike::BeginPlay()
{
	// SetLifeSpan(1);
}

void ASpike::EndPlay()
{
	UE_LOG(LogTopDownARPG, Error, TEXT("Spike is being destroyed"));
}
