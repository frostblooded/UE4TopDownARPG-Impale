// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpaleMovementComponent.h"
#include "TopDownARPG.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UImpaleMovementComponent::UImpaleMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UImpaleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	bAutoActivate = true;
	bIsMovingForward = true;
	bIsMoving = false;
}

void UImpaleMovementComponent::Start(float NewMovementAmplitude)
{
	bIsMoving = true;
	bIsMovingForward = true;
	MovementAmplitude = NewMovementAmplitude;
	StartLocation = GetOwner()->GetActorLocation();
	GoalLocation = StartLocation;
	GoalLocation.Z += MovementAmplitude;

	Movement = GoalLocation - StartLocation;
	Movement.Normalize();
}

// Called every frame
void UImpaleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsMoving) return;

	FVector Location = GetOwner()->GetActorLocation();
	FVector CurrentMovement = Movement * Speed * DeltaTime;

	if (bIsMovingForward)
	{
		if (CurrentMovement.Size() > FVector::Distance(Location, GoalLocation))
		{
			GetOwner()->SetActorLocation(GoalLocation);
		}
		else
		{
			FVector NewLocation = Location + Movement * Speed * DeltaTime;
			GetOwner()->SetActorLocation(NewLocation);
		}

		if (FVector::Distance(Location, GoalLocation) < 1)
		{
			bIsMovingForward = false;
			Movement *= -1;
			OnUpwardMovementEnd.Broadcast();

		}
	}
	else
	{
		if (CurrentMovement.Size() > FVector::Distance(Location, StartLocation))
		{
			GetOwner()->SetActorLocation(StartLocation);
		}
		else
		{
			GetOwner()->SetActorLocation(Location + Movement * Speed * DeltaTime);
		}

		if (FVector::Distance(Location, StartLocation) < 1)
		{
			bIsMoving = false;
			OnMovementEnd.Broadcast();
		}
	}
}

