// Fill out your copyright notice in the Description page of Project Settings.


#include "General/LSPlayerController.h"
#include "Character/LSCharacter.h"
#include "Managers/LaserSimulatorManager.h"
#include "Actors/Computer.h"
#include "Actors/Laser.h"
#include "UI/SettingsWidget.h"

#include "Kismet/GameplayStatics.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "Components/Image.h"
#include "ImageUtils.h"


void ALSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MovementX", this, &ALSPlayerController::InputMovementX);
	InputComponent->BindAxis("MovementY", this, &ALSPlayerController::InputMovementY);

	InputComponent->BindAction("Interact",IE_Pressed, this, &ALSPlayerController::InteractWithObject);
	InputComponent->BindAction("Pause", IE_Pressed, this, &ALSPlayerController::InputPauseMenu);
}

void ALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ALSCharacter>(GetPawn());
	Computer = Cast<AComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), AComputer::StaticClass()));
	Laser = Cast<ALaser>(UGameplayStatics::GetActorOfClass(GetWorld(), ALaser::StaticClass()));
	LaserManager = Cast<ALaserSimulatorManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALaserSimulatorManager::StaticClass()));
}

void ALSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALSPlayerController::InputMovementX(float Value)
{
	if (Character)
	{
		Character->InputMovement.X = Value;
	}
}

void ALSPlayerController::InputMovementY(float Value)
{
	if (Character)
	{
		Character->InputMovement.Y = Value;
	}
}

void ALSPlayerController::InteractWithObject()
{
	if (!Computer || !Laser) 
		return;

	Computer->PCInteract();
	Laser->LaserInteract();
}

void ALSPlayerController::DisableMouseCursor()
{
	bShowMouseCursor = false;

	SetInputMode(FInputModeGameOnly());

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
}

void ALSPlayerController::EnableMouseCursor()
{
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
}

void ALSPlayerController::InputPauseMenu()
{
	if (LaserManager) 
	{
		if (bShouldCreateWidget)
		{
			LaserManager->OpenPauseUI();
			bShouldCreateWidget = false;
		}
		else 
		{
			LaserManager->ClosePauseUI();
			bShouldCreateWidget = true;
		}
	}
}

void ALSPlayerController::LoadImageFromPC(USettingsWidget* InWidget)
{
	if (InWidget) 
	{
		ExportedFilePath = OpenFileDialogue();

		if (!ExportedFilePath.IsEmpty())
		{
			LoadedTexture = LoadedTextureFromFile(ExportedFilePath);

			if (LoadedTexture) 
			{
				InWidget->UpdateImage(LoadedTexture);
			}
		}
	}
}

FString ALSPlayerController::OpenFileDialogue()
{
	TArray<FString> OutFiles;
	IDesktopPlatform* Desktop = FDesktopPlatformModule::Get();

	if (Desktop) 
	{
		Desktop->OpenFileDialog(nullptr, TEXT("Select Image"), TEXT(""), TEXT(""), TEXT("Image Files (*.png;*.jpg)|*.png;*.jpg)"), EFileDialogFlags::None, OutFiles);
	}

	return OutFiles.Num() > 0 ? OutFiles[0] : FString();
}

UTexture2D* ALSPlayerController::LoadedTextureFromFile(const FString& FilePath)
{
	UTexture2D* Texture = nullptr;
	TArray<uint8> RawFileData;

	if (FFileHelper::LoadFileToArray(RawFileData, *FilePath)) 
	{
		Texture = FImageUtils::ImportBufferAsTexture2D(RawFileData);
	}

	return Texture;
}

void ALSPlayerController::StartGravingImage()
{
	if (Laser && bIsFileExport)
	{
		Laser->SpawnTable();
		bIsFileExport = false;
		bCanStartCuting = false;
		bCanStartEngraving = false;
	}
}