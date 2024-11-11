// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Laser.h"
#include "Actors/Computer.h"
#include "Actors/Table.h"
#include "Character/LSCharacter.h"
#include "UI/LaserWidget.h"
#include "General/LSPlayerController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (WSettings) 
	{
		WidgetSettings = CreateWidget<ULaserWidget>(GetWorld(), WSettings);

		if (WidgetSettings)
		{
			WidgetSettings->CloseUI();
		}
	}
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsCharacterOnRange();
	
	if (!bIsCharacterOnRange() )
	{
		//bCanStartLaser = false;
		//bCanOpenCover = false;
	}
}

void ALaser::LaserInteract()
{
	if (!Character)
		return;

	if (!PlayerController)
		return;

	if (WidgetSettings)
	{
		if (Character->bIsTraceWithActor(this)) 
		{
			CanInteractWithLaser = !CanInteractWithLaser;

			if (CanInteractWithLaser)
			{
				
				WidgetSettings->OpenUI();
				PlayerController->EnableMouseCursor(WidgetSettings);
			}
			else
			{
				WidgetSettings->CloseUI();
				PlayerController->DisableMouseCursor();
			}
		}
	}
}

bool ALaser::bIsCharacterOnRange()
{
	if (Character) 
	{
		FVector CharaterLocation = Character->GetActorLocation();
		FVector LaserLocation = GetActorLocation();

		const float DistanceSqr = (CharaterLocation - LaserLocation).SizeSquared2D();

		return DistanceSqr <= (200 * 200) ? true : false;
	}

	return false;
}

void ALaser::SpawnTable()
{
	if (TableToEngravingSpawn && TableToCutSpawn) 
	{
		FVector LaserLocation = GetActorLocation();
		LaserLocation.Y =+ 200;
		LaserLocation.Z =+ 100;
		FRotator LaserRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATable* Table = nullptr;

		if (PlayerController)
		{
			if (PlayerController->bCanStartEngraving)
			{
				Table = GetWorld()->SpawnActor<ATable>(TableToEngravingSpawn, LaserLocation, LaserRotation, SpawnParams);

				
			}
			else if (PlayerController->bCanStartCuting)
			{
				Table = GetWorld()->SpawnActor<ATable>(TableToCutSpawn, LaserLocation, LaserRotation, SpawnParams);
			}
		}
	}
}
