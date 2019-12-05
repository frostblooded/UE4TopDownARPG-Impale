// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TopDownARPGPlayerController.h"
#include "Math/Vector.h"

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
	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpike::OnOverlap);
	RootComponent = BoxComponent;

	ImpaleMovementComponent = CreateDefaultSubobject<UImpaleMovementComponent>("ImpaleMovement");
	ImpaleMovementComponent->OnUpwardMovementEnd.AddUObject(this, &ASpike::TrySpawnNextSpike);
	ImpaleMovementComponent->OnMovementEnd.AddUObject(this, &ASpike::DestroySelf);
}

void ASpike::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(Other) == false) {
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::OnOverlap() - IsValid(Other) == false"));
	}

	APawn* OtherPawn = (APawn*)Other;
	OtherPawn->GetMovementComponent()->Deactivate();
	OtherPawn->FindComponentByClass<UCapsuleComponent>()->SetGenerateOverlapEvents(false);

	UImpaleMovementComponent* OtherImpaleMovementComponent = Other->FindComponentByClass<UImpaleMovementComponent>();

	if (OtherImpaleMovementComponent != nullptr)
	{
		OtherImpaleMovementComponent->Start(GetBoxHeight() * 2);
	}
}

void ASpike::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(GetSpawnLocation());
	ImpaleMovementComponent->Start(GetBoxHeight());
}

FVector ASpike::GetSpawnLocation() {
	FVector Start = GetActorLocation();

	// This is needed because if there was a previous spike, it
	// was destroyed underground.
	Start.Z += GetBoxHeight();

	FVector End = Start + FVector::DownVector * 1000;
	FCollisionQueryParams CollisionParams;

	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::SpawnNextSpike() - IsValid(World) == false"));
	}

	FHitResult OutHit;
	bool bIsHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);

	FVector SpawnLocation;

	if (bIsHit)
	{
		SpawnLocation = OutHit.ImpactPoint;
		SpawnLocation.Z += BoxComponent->GetScaledBoxExtent().Z;
	}
	else
	{
		SpawnLocation = GetTransform().GetLocation();
	}

	// Start below the ground
	SpawnLocation.Z -= GetBoxHeight();
	
	return SpawnLocation;
}

void ASpike::TrySpawnNextSpike()
{
	if (RemainingSpikes > 0)
	{
		SpawnNextSpike();
	}
}

void ASpike::DestroySelf()
{
	Destroy();
}

void ASpike::SpawnNextSpike()
{
	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::SpawnNextSpike() - IsValid(World) == false"));
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = GetInstigator();

	FTransform Transform = GetTransform();
	FVector NewLocation = Transform.GetLocation() + GetActorForwardVector() * 50;

	ASpike* NewSpike = World->SpawnActor<ASpike>(GetClass(), NewLocation, Transform.GetRotation().Rotator(), SpawnParams);

	if (IsValid(NewSpike) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::SpawnNextSpike() - IsValid(NewSpike) == false"));
	}

	NewSpike->RemainingSpikes = RemainingSpikes - 1;
}

float ASpike::GetBoxHeight()
{
	return BoxComponent->GetScaledBoxExtent().Z * 2;
}

