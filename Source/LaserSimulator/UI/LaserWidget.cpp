// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LaserWidget.h"
#include "General/LSPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void ULaserWidget::CloseUI()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void ULaserWidget::OpenUI()
{
	SetVisibility(ESlateVisibility::Visible);
	AddToViewport();
}

void ULaserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseUIButton && StartEngravingButton)
	{
		CloseUIButton->OnClicked.AddDynamic(this, &ULaserWidget::OnButtonCliked);
		StartEngravingButton->OnClicked.AddDynamic(this, &ULaserWidget::StartEngraving);
		StoptEngravingButton->OnClicked.AddDynamic(this, &ULaserWidget::StopEngraving);
	}
}

void ULaserWidget::StartEngraving()
{
	if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (PlayerController->bIsFileExport) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Start operation"));
			PlayerController->PlayGraving();
			PlayerController->DisableMouseCursor();
			CloseUI();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cant Start operation"));
		}
	}
}

void ULaserWidget::StopEngraving()
{
	if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) 
	{
		PlayerController->StopGraving();
	}
}

void ULaserWidget::OnButtonCliked()
{
	CloseUI();
}