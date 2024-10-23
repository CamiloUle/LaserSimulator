// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LSCharacter.h"
#include "Camera/LSCameraActor.h"
#include "Actors/Computer.h"
#include "Actors/Laser.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALSCharacter::ALSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Camera = Cast<ALSCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALSCameraActor::StaticClass()));
	Computer = Cast<AComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), AComputer::StaticClass()));
	Laser = Cast<ALaser>(UGameplayStatics::GetActorOfClass(GetWorld(), ALaser::StaticClass()));
}

void ALSCharacter::OpenUI()
{

}

// Called every frame
void ALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Camera)
	{
		FRotator CameraRotation = Camera->GetActorRotation();

		//Set Charater MovementDirection based on the derection of the camera
		FVector ForwardDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);

		FVector MovementDirection = (ForwardDirection * InputMovement.X) + (RightDirection * InputMovement.Y);
		AddMovementInput(MovementDirection);

		if (MovementDirection.Size2D() > 0)
		{
			LastMovementDirection = MovementDirection;
		}
	}

	bIsTraceWithActor(this);
}

// Called to bind functionality to input
void ALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ALSCharacter::bIsTraceWithActor(AActor* OtherActor)
{
	if (OtherActor && Camera)
	{
		FCollisionQueryParams QueryParams;
		FHitResult OutHit;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(Camera);

		TEnumAsByte<enum ECollisionChannel> CustomHitCollisionChannnel = ECC_GameTraceChannel1;

		if (CustomHitCollisionChannnel)
		{
			FVector CameraPosition = Camera->GetActorLocation();
			FRotator CameraRotation = Camera->GetActorRotation();

			FVector StarPosition = CameraPosition;
			FVector EndPosition = StarPosition + (CameraRotation.Vector()) * 400.0f;

			DrawDebugLine(GetWorld(), StarPosition, EndPosition, FColor::Emerald, false, 0.05f, 0.0f, 1.0f);

			bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, StarPosition, EndPosition, CustomHitCollisionChannnel, QueryParams);

			if (bIsHit)
			{
				if (OutHit.GetActor()->GetActorNameOrLabel() == OtherActor->GetActorNameOrLabel())
				{
					return bIsHit;
				}
			}
		}
	}

	return false;
}

