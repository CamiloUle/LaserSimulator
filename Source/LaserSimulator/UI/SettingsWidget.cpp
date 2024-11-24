// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SettingsWidget.h"
#include "General/LSPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Image.h"

void USettingsWidget::CloseUI()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void USettingsWidget::OpenUI()
{
	SetVisibility(ESlateVisibility::Visible);
	AddToViewport();
}

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseUIButton && OpenFileButton && ExportFileButton && CutFileButton && GrabFileButton)
	{
		CloseUIButton->OnClicked.AddDynamic(this, &USettingsWidget::OnButtonCliked);
		OpenFileButton->OnClicked.AddDynamic(this, &USettingsWidget::OnButtonOpenFileCliked);
		ExportFileButton->OnClicked.AddDynamic(this, &USettingsWidget::ExportFile);
		CutFileButton->OnClicked.AddDynamic(this, &USettingsWidget::CutFile);
		GrabFileButton->OnClicked.AddDynamic(this, &USettingsWidget::GrabFile);
	}
}

void USettingsWidget::OnButtonCliked()
{
	CloseUI();
}

void USettingsWidget::OnButtonOpenFileCliked()
{
	if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) 
	{
		PlayerController->LoadImageFromPC(this);
	}
}

void USettingsWidget::UpdateImage(UTexture2D* LoadedTexture)
{
	if (LoadedTexture && ImageWidget) 
	{
		ImageWidget->SetOpacity(1.f);
		//eImageWidget->SetBrushTintColor()
		ImageWidget->SetBrushFromTexture(LoadedTexture);
	}
}

void USettingsWidget::GrabFile()
{
	if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) 
	{
		if (!PlayerController->ExportedFilePath.IsEmpty()) 
		{
			PlayerController->bCanStartEngraving = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("The file is for graving"));
		
		}
		else 
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("You must select a file"));
		}
	}
}

void USettingsWidget::CutFile()
{
	if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (!PlayerController->ExportedFilePath.IsEmpty())
		{
			PlayerController->bCanStartCuting = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("The file is for cutting"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("You must select a file"));
		}
	}
}

void USettingsWidget::ExportFile()
{
	if (!ImageWidget)
		return;

	if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (PlayerController->bCanStartEngraving && !PlayerController->bCanStartCuting
			|| !PlayerController->bCanStartEngraving && PlayerController->bCanStartCuting)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("File was exported"));
			PlayerController->bIsFileExport = true;
			ImageWidget->SetOpacity(0.f);
		}
		else if (PlayerController->bCanStartEngraving && PlayerController->bCanStartCuting)
		{
			PlayerController->bCanStartEngraving = false;
			PlayerController->bCanStartCuting = false;
			PlayerController->bIsFileExport = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You can only export the file for cutting or Engraving."));
		}
		else if (!PlayerController->ExportedFilePath.IsEmpty() && !PlayerController->bCanStartCuting ||
			     !PlayerController->ExportedFilePath.IsEmpty() && !PlayerController->bCanStartEngraving)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("you must select a type of action."));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Dont have file"));
		}
	}
}
