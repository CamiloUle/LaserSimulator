// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Door.h"
#include "Character/LSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	Character = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Character) 
	{
		if (Character->bIsTraceWithActor(this)) 
		{
			OnOpenDoor();
		}
	}
}

