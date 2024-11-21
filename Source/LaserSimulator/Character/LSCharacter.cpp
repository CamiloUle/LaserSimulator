// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LSCharacter.h"
#include "Camera/LSCameraActor.h"
#include "Actors/Computer.h"
#include "Actors/Laser.h"
#include "Actors/Table.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALSCharacter::ALSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("HandleComponent"));
}

// Called when the game starts or when spawned
void ALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Camera = Cast<ALSCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALSCameraActor::StaticClass()));
	Computer = Cast<AComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), AComputer::StaticClass()));
	Laser = Cast<ALaser>(UGameplayStatics::GetActorOfClass(GetWorld(), ALaser::StaticClass()));
	Table = Cast<ATable>(UGameplayStatics::GetActorOfClass(GetWorld(), ATable::StaticClass()));
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

	if (PhysicsHandle && Camera)
	{
		if (PhysicsHandle->GetGrabbedComponent() != nullptr) 
		{
			FVector CameraPosition = Camera->GetActorLocation();
			FRotator CameraRotation = Camera->GetActorRotation();

			FVector StarPosition = CameraPosition;
			FVector EndPosition = StarPosition + (CameraRotation.Vector()) * 100.0f;

			PhysicsHandle->SetTargetLocationAndRotation(EndPosition, CameraRotation);
		}
	}
}

// Called to bind functionality to input
void ALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ALSCharacter::bIsTraceWithActor(AActor* OtherActor)
{
	if (OtherActor && Camera && Laser)
	{
		FCollisionQueryParams QueryParams;
		FHitResult OutHit;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(Camera);

		TEnumAsByte<enum ECollisionChannel> CustomHitCollisionChannnel = ECC_GameTraceChannel2;

		if (CustomHitCollisionChannnel)
		{
			FVector CameraPosition = Camera->GetActorLocation();
			FRotator CameraRotation = Camera->GetActorRotation();

			FVector StarPosition = CameraPosition;
			FVector EndPosition = StarPosition + (CameraRotation.Vector()) * 200.0f;

			bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, StarPosition, EndPosition, CustomHitCollisionChannnel, QueryParams);

			if (bIsHit)
			{
				if (OutHit.GetActor()->GetActorNameOrLabel() == OtherActor->GetActorNameOrLabel())
				{
					return bIsHit;
				}
			}
			

			bool bIsHitComponents = false;

			if (OutHit.GetComponent())
			{
				if (OutHit.GetComponent()->GetName() == "CoverChild")
				{
					Laser->bCanOpenCover = true;
					bIsHitComponents = true;
				}
				else if (OutHit.GetComponent()->GetName() == "Laser")
				{
					Laser->bCanStartLaser = true;
					bIsHitComponents = true;
				}
				else if (OutHit.GetComponent()->GetName() == "ControlPanel")
				{
					Laser->bIsTraceWithPanel = true;
					bIsHitComponents = true;
				}
				else if (OutHit.GetComponent()->GetName() == "Computer")
				{
					Computer->bCanChangeMaterial = true;
					bIsHitComponents = true;
				}
				else if (OutHit.GetComponent()->GetName() == "Table")
				{
					Table->bCanChangeMaterial = true;
					bIsHitComponents = true;
				}
			}

			if (!bIsHitComponents)
			{
				Laser->bCanOpenCover = false;
				Laser->bCanStartLaser = false;
				Laser->bIsTraceWithPanel = false;
				Computer->bCanChangeMaterial = false;
				Table->bCanChangeMaterial = false;
			}
		}
	}

	return false;
}

void ALSCharacter::GrabObject()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FCollisionQueryParams QueryParams;
	FHitResult OutHit;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Camera);

	TEnumAsByte<enum ECollisionChannel> CustomHitCollisionChannnel = ECC_GameTraceChannel2;

	if (Camera) 
	{
		if (CustomHitCollisionChannnel)
		{
			FVector CameraPosition = Camera->GetActorLocation();
			FRotator CameraRotation = Camera->GetActorRotation();

			FVector StarPosition = CameraPosition;
			FVector EndPosition = StarPosition + (CameraRotation.Vector()) * 200.0f;

			bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, StarPosition, EndPosition, CustomHitCollisionChannnel, QueryParams);

			if (bIsHit)
			{
				UPrimitiveComponent* HitComponent = OutHit.GetComponent();

				if (HitComponent) 
				{
					HitComponent->WakeAllRigidBodies();

					PhysicsHandle->GrabComponentAtLocationWithRotation(
						HitComponent,
						NAME_None,
						OutHit.ImpactPoint,
						OutHit.GetComponent()->GetComponentRotation());
				}
			}
		}
	}
}

void ALSCharacter::ReleaseObject()
{
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->ReleaseComponent();
	}
}