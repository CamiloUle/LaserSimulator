// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERSIMULATOR_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(Transient)
	FString FilePath;

	UPROPERTY(Transient)
	bool bCanExportFile = false;

public:

	UFUNCTION(BlueprintCallable)
	void CloseUI();

	UFUNCTION(BlueprintCallable)
	void OpenUI();

	UFUNCTION(BlueprintCallable)
	void ExportFile();

	UFUNCTION(BlueprintCallable)
	void GrabFile();

	UFUNCTION(BlueprintCallable)
	void CutFile();

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseUIButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenFileButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExportFileButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GrabFileButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CutFileButton;

	UFUNCTION()
	void OnButtonCliked();

	UFUNCTION()
	void OnButtonOpenFileCliked();

	UFUNCTION(BlueprintCallable)
	void UpdtadeImage(UTexture2D* LoadedTexture);

	UPROPERTY(meta = (BindWidget))
	class UImage* ImageWidget;

protected:

	virtual void NativeConstruct() override;

};
