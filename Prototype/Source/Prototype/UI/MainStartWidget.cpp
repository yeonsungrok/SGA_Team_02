// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainStartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainStartWidget::NativeConstruct()
{

	Super::NativeConstruct();



	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainStartWidget::StartButtonClick);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainStartWidget::QuitButtonClick);
	}
}

void UMainStartWidget::StartButtonClick()
{

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
}

void UMainStartWidget::QuitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
