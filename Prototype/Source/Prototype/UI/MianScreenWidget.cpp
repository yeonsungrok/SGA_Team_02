// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MianScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMianScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();



	if (TestButton)
	{
		TestButton->OnClicked.AddDynamic(this, &UMianScreenWidget::TestButtonClick);
	}
}

void UMianScreenWidget::TestButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
}
