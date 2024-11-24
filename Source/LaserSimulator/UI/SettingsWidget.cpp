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
		}
		else 
		{
			OnMessageDontHaveFile();
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
		}
		else
		{
			OnMessageDontHaveFile();
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
			OnMessageExportFile();
			PlayerController->bIsFileExport = true;
			ImageWidget->SetOpacity(0.f);
		}
		else if (PlayerController->bCanStartEngraving && PlayerController->bCanStartCuting)
		{
			PlayerController->bCanStartEngraving = false;
			PlayerController->bCanStartCuting = false;
			PlayerController->bIsFileExport = false;
			OnMessageOnlyOneAction();
		}
		else if (!PlayerController->ExportedFilePath.IsEmpty() && !PlayerController->bCanStartCuting ||
			     !PlayerController->ExportedFilePath.IsEmpty() && !PlayerController->bCanStartEngraving)
		{
			OnMessageTypeOfAction();
		}
		else
		{
			OnMessageDontHaveFile();
		}
	}
}
