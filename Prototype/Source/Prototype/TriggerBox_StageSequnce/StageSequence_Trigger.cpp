// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerBox_StageSequnce/StageSequence_Trigger.h"
#include "Base/MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "Base/Stage1BossGameModeBase.h"

AStageSequence_Trigger::AStageSequence_Trigger()
{
    PrimaryActorTick.bCanEverTick = false;

}

void AStageSequence_Trigger::BeginPlay()
{
    Super::BeginPlay();

}

void AStageSequence_Trigger::OnSequenceFinished()
{
   Super::OnSequenceFinished();
   UIManager->OpenUI(UI_LIST::Boss);

    AStage1BossGameModeBase *GameMode = Cast<AStage1BossGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->BossStart();
    }
}


