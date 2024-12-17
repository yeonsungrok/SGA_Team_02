// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"
#include "Base/MyGameInstance.h"
#include "Player/MyPlayer.h"
#include "UI/MainStartWidget.h"
#include "Blueprint/UserWidget.h" 
#include "Base/Managers/UIManager.h"

#include "Kismet/GameplayStatics.h"

AStartGameModeBase::AStartGameModeBase()
{

}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//GAMEINSTANCE->InitializeManagers();

	if (UIManager)
	{
		auto startUI = UIManager->GetStartUI();
		if (startUI != nullptr)
		{
			UIManager->OpenUI(UI_LIST::StartUI);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StartUI is nullptr in UIManager!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UIManager is nullptr! Check its initialization."));
	}

}

void AStartGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
