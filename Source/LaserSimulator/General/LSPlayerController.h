// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LSPlayerController.generated.h"

class USettingsWidget;

/**
 * 
 */
UCLASS()
class LASERSIMULATOR_API ALSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void InputMovementX(float Value);
	void InputMovementY(float Value);

	void PlayInteractions();
	void StopGrabObject();

	void PlayGraving();
	void StopGraving();

	UFUNCTION(BlueprintCallable)
	void DisableMouseCursor();

	UFUNCTION(BlueprintCallable)
	void EnableMouseCursor(UUserWidget* WidgetToFocus);

	void InputPauseMenu();
	
	void LoadImageFromPC(USettingsWidget* InWidget);

	void RestoreValues();

	UPROPERTY(Transient)
	FVector2D InputMovement = FVector2D::ZeroVector;

	UPROPERTY(Transient)
	bool bShouldCreateWidget = true;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bCanStartEngraving = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bCanStartCuting = false;

	UPROPERTY(Transient)
	bool bIsFileExport = false;

	UPROPERTY(Transient)
	FString ExportedFilePath;

public:
	
	class ALSCharacter* Character = nullptr;

	UPROPERTY(Transient)
	class AComputer* Computer;

	UPROPERTY(Transient)
	class ALaser* Laser;

	UPROPERTY(Transient)
	class ALaserSimulatorManager* LaserManager;

	static FString OpenFileDialogue();
	static UTexture2D* LoadedTextureFromFile(const FString& FilePath);

	UPROPERTY(Transient,BlueprintReadWrite)
	UTexture2D* LoadedTexture;

private:

	virtual void Tick(float DeltaTime) override;
	void SetupInputComponent() override;
	virtual void BeginPlay() override;
};
