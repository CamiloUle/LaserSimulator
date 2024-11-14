// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "LSCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class LASERSIMULATOR_API ALSCameraActor : public ACameraActor
{
	GENERATED_BODY()
	

public:

	ALSCameraActor();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float XGap = 700.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float CameraHeight = 0.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float CameraSpeed = 700.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float DeltaX = 0.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float DeltaY = 0.f;

protected:

	UPROPERTY(Transient)
	class ALSCharacter* Character;

	UPROPERTY(Transient)
	class ALSPlayerController* PC;

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
