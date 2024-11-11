// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LSCameraActor.h"
#include "Character/LSCharacter.h"
#include "General/LSPlayerController.h"

#include "Kismet/GameplayStatics.h"

ALSCameraActor::ALSCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALSCameraActor::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PC = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ALSCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PC && Character)
	{
		FVector TargetLocation = Character->GetActorLocation();
		FVector NewCameraLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation + FVector(-XGap, 0.f, 0.f), DeltaTime, CameraSpeed);
		NewCameraLocation.Z = 120.f;
		SetActorLocation(NewCameraLocation);

		PC->GetInputMouseDelta(DeltaX, DeltaY);

		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += DeltaX;
		NewRotation.Pitch += DeltaY;
		SetActorRotation(NewRotation);

		Character->MeshComp->SetRelativeRotation(NewRotation);
	}
}
