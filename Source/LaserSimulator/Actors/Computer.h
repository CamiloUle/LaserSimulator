// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Computer.generated.h"

UCLASS()
class LASERSIMULATOR_API AComputer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComputer();

	UPROPERTY(Transient)
	class ALSCharacter* Character;

	UPROPERTY(Transient)
	bool CanInteractWithPc = false;

	UPROPERTY(Transient)
	bool bIsComputerOn = false;

	UPROPERTY(Transient, BlueprintReadWrite)
	bool bCanChangeMaterial = false;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<class USettingsWidget> WSettings;

	UPROPERTY(Transient)
	class USettingsWidget* WidgetSettings;

	UPROPERTY(Transient)
	class ALSPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PCInteract();
	bool bIsInCharacterRange();

};
