// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/MyGameModeBase.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Base/Managers/UIManager.h"
#include "Player/MyPlayer.h"

void AMyGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    InitializeGameInstance();
    InitializePlayer();
}

void AMyGameModeBase::InitializePlayer()
{
    Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        GameInstance = Cast<UMyGameInstance>(GetGameInstance());
        UStatComponent *StatComponent = Player->FindComponentByClass<UStatComponent>();
        UInventoryComponent *InvenComponent = Player->FindComponentByClass<UInventoryComponent>();

        if (GameInstance)
        {
            if (GameInstance->GetFirst())
            {
                GameInstance->SetFirst(false);
                if (StatComponent)
                {
                    Player->GetStatComponent()->SetLevelInit(1);
                }
                if (InvenComponent)
                {
                    Player->GetInventory()->InitSlot();
                }
            }
            else
            {
                GameInstance->LoadPlayerSkeletal(Player);

                if (StatComponent)
                {
                    GameInstance->LoadPlayerStats(StatComponent);
                }

                if (InvenComponent)
                {
                    GameInstance->LoadInventory(InvenComponent);
                }

                Player->GetStatComponent()->Reset();
            }
        }

        APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
        }
    }
}

void AMyGameModeBase::InitializeGameInstance()
{
    GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->InitializeManagers();
    }
}
