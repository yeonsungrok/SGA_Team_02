// Fill out your copyright notice in the Description page of Project Settings.

#include "StartGameModeBase.h"
#include "Base/MyGameInstance.h"
#include "Player/MyPlayer.h"
#include "UI/MainStartWidget.h"
#include "UI/LoginWidget.h"
#include "Blueprint/UserWidget.h"
#include "Base/Managers/UIManager.h"

#include "Kismet/GameplayStatics.h"

AStartGameModeBase::AStartGameModeBase()
{
}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}

	if (UIManager)
	{
		UIManager->OpenUI(UI_LIST::Login);
	}

	GAMEINSTANCE->OnLoginSuccess.AddDynamic(this, &AStartGameModeBase::OnLoginSuccess);
}

void AStartGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AStartGameModeBase::OnLoginSuccess()
{
    if (UIManager)
    {
		UIManager->CloseUI(UI_LIST::Login);
        UIManager->OpenUI(UI_LIST::StartUI);
    }
}