// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/LaserSimulatorManager.h"
#include "General/LSPlayerController.h"
#include "UI/PauseWidget.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALaserSimulatorManager::ALaserSimulatorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaserSimulatorManager::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PauseWidgetInstance)
	{
		PauseWidget = CreateWidget<UPauseWidget>(GetWorld(), PauseWidgetInstance);

		if (PauseWidget) 
		{
			PauseWidget->HideUI();
		}
	}

}

// Called every frame
void ALaserSimulatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserSimulatorManager::OpenPauseUI()
{
	if (PauseWidget && PlayerController)
	{
		PauseWidget->ShowUI();
		PlayerController->EnableMouseCursor(PauseWidget);
	}
}

