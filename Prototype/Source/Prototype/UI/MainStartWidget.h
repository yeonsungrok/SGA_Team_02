// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UMainStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainStartWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SoundButton;

	UFUNCTION()
	void StartButtonClick();

	UFUNCTION()
	void QuitButtonClick();

	UFUNCTION()
	void SoundButtonClick();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> _Keyimage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> _Sound;

};
