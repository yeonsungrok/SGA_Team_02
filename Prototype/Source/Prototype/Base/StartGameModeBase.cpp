// Fill out your copyright notice in the Description page of Project Settings.

#include "StartGameModeBase.h"
#include "Base/MyGameInstance.h"
#include "Player/MyPlayer.h"
#include "UI/MainStartWidget.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"

AStartGameModeBase::AStartGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UMainStartWidget> StartWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/MainStart_UI.MainStart_UI_C'"));
	if (StartWidget.Succeeded())
	{
		StartLevelWidgetClass = StartWidget.Class;
	}
}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (StartLevelWidgetClass)
	{
		StartLevelWidget = CreateWidget<UMainStartWidget>(GetWorld(), StartLevelWidgetClass);

		if (StartLevelWidget)
		{
			StartLevelWidget->AddToViewport();
		}
	}
}

void AStartGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
