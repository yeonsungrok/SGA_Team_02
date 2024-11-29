// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"
#include "Base/MyGameInstance.h"
#include "UI/MianScreenWidget.h"
#include "Blueprint/UserWidget.h" 
#include "UObject/ConstructorHelpers.h"

AStartGameModeBase::AStartGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UMianScreenWidget> StartWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Map/NewBlueprint.NewBlueprint_C'"));
	if (StartWidget.Succeeded())
	{
		StartLevelWidgetClass = StartWidget.Class;
	}

}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GAMEINSTANCE->InitializeManagers(); // ?
	if (StartLevelWidgetClass)
	{

		StartLevelWidget = CreateWidget<UMianScreenWidget>(GetWorld(), StartLevelWidgetClass);

		if (StartLevelWidget)
		{
			StartLevelWidget->AddToViewport(10);
		}
		
	}
}

void AStartGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
