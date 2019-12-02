// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "DrawDebugHelpers.h"

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
	UE_LOG(LogTopDownARPG, Display, TEXT("Overlapped with %s"), *GetNameSafe(Other));

	if (IsValid(Other) == false) {
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::OnOverlap() - IsValid(Other) == false"));
	}
}

void ASpike::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(GetSpawnLocation());
	SetLifeSpan(LifeSpan);
}

void ASpike::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// We only want to spawn a new spike if the previous was destroyed
	// normally. We don't want to spawn a new spike if the level is being
	// unloaded or the game is quitting or something like that.
	if (EndPlayReason == EEndPlayReason::Type::Destroyed)
	{
		if (RemainingSpikes > 0)
		{
			SpawnNextSpike();
		}
	}

	Super::EndPlay(EndPlayReason);
}

FVector ASpike::GetSpawnLocation() {
	FVector Start = GetTransform().GetLocation();
	FVector ForwardVector = FVector::DownVector;
	FVector End = Start + ForwardVector * 1000;
	FCollisionQueryParams CollisionParams;
	FCollisionShape BoxShape = FCollisionShape::MakeBox(BoxComponent->GetScaledBoxExtent());

	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("ASpike::SpawnNextSpike() - IsValid(World) == false"));
	}

	FHitResult OutHit;
	bool bIsHit = World->SweepSingleByChannel(OutHit, Start, End, ForwardVector.ToOrientationQuat(), ECollisionChannel::ECC_Visibility, BoxShape, CollisionParams);

	FVector SpawnLocation;

	if (bIsHit)
	{
		SpawnLocation = OutHit.Location;
	}
	else
	{
		SpawnLocation = GetTransform().GetLocation();
	}
	
	return SpawnLocation;
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
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
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
