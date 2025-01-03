// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "LSCharacter.generated.h"

class UPhysicsHandleComponent;

UCLASS()
class LASERSIMULATOR_API ALSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALSCharacter();


public:

	UPROPERTY(Transient, BlueprintReadOnly)
	FVector InputMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LastMovementDirection;

	UPROPERTY(Transient)
	class ALSCameraActor* Camera;

	UPROPERTY(Transient)
	class AComputer* Computer;

	UPROPERTY(Transient)
	class ALaser* Laser;

	UPROPERTY(Transient)
	class ATable* Table;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;

	bool bIsTraceWithActor(AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimHand();

	void GrabObject();
	void ReleaseObject();
};
