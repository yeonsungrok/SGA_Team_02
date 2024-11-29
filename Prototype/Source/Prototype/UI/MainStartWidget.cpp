// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainStartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMainStartWidget::NativeConstruct()
{

	Super::NativeConstruct();



	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainStartWidget::StartButtonClick);
	}
}

void UMainStartWidget::StartButtonClick()
{

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
}
