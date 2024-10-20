// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Laser.h"
#include "Character/LSCharacter.h"
#include "Actors/Computer.h"
#include "UI/SettingsWidget.h"
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
		WidgetSettings = CreateWidget<USettingsWidget>(GetWorld(), WSettings);

		if (WidgetSettings)
		{
			WidgetSettings->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsCharacterOnRange();

	if (WidgetSettings && PlayerController) 
	{
		if (!bIsCharacterOnRange() && WidgetSettings->Visibility != ESlateVisibility::Hidden)
		{
			WidgetSettings->SetVisibility(ESlateVisibility::Hidden);
			WidgetSettings->RemoveFromParent();

			PlayerController->bShowMouseCursor = false;
		}
	}
}

void ALaser::LaserInteract()
{
	if (!Character || !PlayerController || !WidgetSettings)
		return;

	bool bHideWidget = false;

	if (Character->bIsTraceWithActor() && WidgetSettings && PlayerController)
	{
		if (CanInteractWithLaser && bIsCharacterOnRange())
		{
			bHideWidget = !bHideWidget;

			if (bHideWidget) 
			{
				if (WidgetSettings->Visibility != ESlateVisibility::Visible)
				{
					WidgetSettings->SetVisibility(ESlateVisibility::Visible);
					WidgetSettings->AddToViewport();

					PlayerController->bShowMouseCursor = true;
				}
			}
			else 
			{
				if (WidgetSettings->Visibility != ESlateVisibility::Hidden)
				{
					WidgetSettings->SetVisibility(ESlateVisibility::Hidden);
					WidgetSettings->RemoveFromParent();

					PlayerController->bShowMouseCursor = false;
				}
			}
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("bHideWidget: %s"), bHideWidget ? TEXT("true") : TEXT("false")));
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