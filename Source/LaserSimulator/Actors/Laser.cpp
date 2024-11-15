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
	}
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsCharacterOnRange();
}

void ALaser::LaserInteract()
{
	if (!Character)
		return;

	if (!PlayerController)
		return;

	if (WidgetSettings && bIsTraceWithPanel)
	{
		if (Character->bIsTraceWithActor(this)) 
		{
			WidgetSettings->OpenUI();
			PlayerController->EnableMouseCursor(WidgetSettings);
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
	if (TableToSpawn)
	{
		FVector LaserLocation = GetActorLocation();
		LaserLocation.X =+ 20;
		LaserLocation.Z =+ 100;
		FRotator LaserRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		
		if (PlayerController) 
		{
			if (PlayerController->bCanStartCuting || PlayerController->bCanStartEngraving) 
			{
				ATable* Table = GetWorld()->SpawnActor<ATable>(TableToSpawn, LaserLocation, LaserRotation, SpawnParams);
				//PlayerController->RestoreValues();
			}
		}
	}
}
