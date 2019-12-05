// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ImpaleMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWNARPG_API UImpaleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UImpaleMovementComponent();

	void Start(float NewMovementAmplitude);

	DECLARE_MULTICAST_DELEGATE(OnUpwardMovementEndSignature);
	OnUpwardMovementEndSignature OnUpwardMovementEnd;

	DECLARE_MULTICAST_DELEGATE(OnMovementEndSignature);
	OnMovementEndSignature OnMovementEnd;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float Speed = 1000;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float MovementAmplitude = 100;
	FVector StartLocation;
	FVector Movement;
	FVector GoalLocation;
	bool bIsMovingForward = true;
	bool bIsMoving = false;
};
