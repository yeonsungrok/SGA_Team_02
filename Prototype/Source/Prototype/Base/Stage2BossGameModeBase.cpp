// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/Stage2BossGameModeBase.h"
#include "MyGameInstance.h"
#include "../Base/Managers/UIManager.h"
#include "../Monster/Boss2Monster.h"
#include "../Monster/AI/AIController_Boss2.h"
#include "UI/Boss2Widget.h"


AStage2BossGameModeBase::AStage2BossGameModeBase()
{
    static ConstructorHelpers::FClassFinder<ABoss2Monster> BM(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Monster/BossMonster/Boss2_BP.Boss2_BP_C'"));
	if (BM.Succeeded())
	{
		_boss = BM.Class;
	}

	_portalLocation = FVector(40.0f, -3230.1f, 190.9f);
	_portalRotation = FRotator::ZeroRotator;
	_playerLocation = FVector(50.0f, -1420.f, 226.f);
	_playerRotation = FRotator::ZeroRotator;

}

void AStage2BossGameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void AStage2BossGameModeBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void AStage2BossGameModeBase::BossClear()
{
	Super::BossClear();
	GAMEINSTANCE->SetStage2Clear(true);
}

void AStage2BossGameModeBase::BossStart()
{
	Super::BossStart();
	UIManager->OpenUI(UI_LIST::Boss2);
}

void AStage2BossGameModeBase::SpawnBoss()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("Boss2");
	ABoss2Monster* Boss = GetWorld()->SpawnActor<ABoss2Monster>(_boss, FVector(141.0f, -3893.1f, 429.9f), FRotator::ZeroRotator, SpawnParams);
	if (Boss)
	{
		Boss->GetStatComponent()->SetBossLevelInit(1);
		AAIController_Boss2* BossAI = GetWorld()->SpawnActor<AAIController_Boss2>(AAIController_Boss2::StaticClass());
		if (BossAI)
		{
			BossAI->OnPossess(Boss);
		}

		Boss->GetStatComponent()->_PlHPDelegate.AddUObject(UIManager->GetBoss2UI(), &UBoss2Widget::UpdateBossHPBar);
		Boss->GetStatComponent()->_deathDelegate.AddUObject(this, &AStage2BossGameModeBase::BossClear);
	}

}



