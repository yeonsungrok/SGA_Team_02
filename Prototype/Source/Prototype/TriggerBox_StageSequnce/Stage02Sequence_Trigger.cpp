// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBox_StageSequnce/Stage02Sequence_Trigger.h"
#include "Base/MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "Base/Stage2BossGameModeBase.h"

AStage02Sequence_Trigger::AStage02Sequence_Trigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStage02Sequence_Trigger::BeginPlay()
{
	Super::BeginPlay();

}

void AStage02Sequence_Trigger::OnSequenceFinished()
{
    Super::OnSequenceFinished();

	UIManager->OpenUI(UI_LIST::Boss2);

    AStage2BossGameModeBase *GameMode = Cast<AStage2BossGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->BossStart();
    }
}




