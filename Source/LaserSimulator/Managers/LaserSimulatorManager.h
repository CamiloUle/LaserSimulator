// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserSimulatorManager.generated.h"

UCLASS()
class LASERSIMULATOR_API ALaserSimulatorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserSimulatorManager();

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<class UPauseWidget> PauseWidgetInstance;

	UPROPERTY(Transient)
	class UPauseWidget* PauseWidget;

	UPROPERTY(Transient)
	class ALSPlayerController* PlayerController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenPauseUI();

};
