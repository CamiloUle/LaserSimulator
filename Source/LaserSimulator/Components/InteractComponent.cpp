// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Character/LSCharacter.h"
#include "UI/InteractableWidget.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "ActorToInteract", ActorList);

	for (const FInteractableInfo ActorInfo : InteracInfo) 
	{
		if(ActorInfo.InteractableUI)
		{
			UI = CreateWidget<UInteractableWidget>(GetWorld(), ActorInfo.InteractableUI);

			if (UI) 
			{
				UI->AddToViewport();
				UI->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ShowUI();
}

void UInteractComponent::ShowUI()
{
	if (Character)
	{
		for (const FInteractableInfo ActorInfo : InteracInfo) 
		{
			if (ActorInfo.InteractableUI && IsValid(UI)) 
			{
				FVector CharacterLocation = Character->GetActorLocation();

				if (ActorList.Num() > 0)
				{
					bool CanShowWidget = false;

					for (AActor* Actor : ActorList)
					{
						FVector ActorLocation = Actor->GetActorLocation();

						const float DistanceSqr = (ActorLocation - CharacterLocation).SizeSquared2D();

						if (DistanceSqr <= (ActorInfo.DistanceToInteract * ActorInfo.DistanceToInteract) && Character->bIsTraceWithActor(Actor))
						{
							CanShowWidget = true;
							break;
						}
					}

					if (CanShowWidget && UI->Visibility != ESlateVisibility::Visible) 
					{
						UI->SetVisibility(ESlateVisibility::Visible);
					}
					else if (!CanShowWidget && UI->Visibility != ESlateVisibility::Hidden)
					{
						UI->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		}
	}
}

