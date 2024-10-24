// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class LASERSIMULATOR_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

	UPROPERTY(Transient)
	class ALSCharacter* Character;

	UPROPERTY(Transient)
	bool CanInteractWithLaser = false;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<class ULaserWidget> WSettings;

	UPROPERTY(Transient)
	class ULaserWidget* WidgetSettings;

	UPROPERTY(Transient)
	class ALSPlayerController* PlayerController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaserInteract();

	bool bIsCharacterOnRange();
};
