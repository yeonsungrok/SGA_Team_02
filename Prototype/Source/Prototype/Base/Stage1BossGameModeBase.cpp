// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/Stage1BossGameModeBase.h"
#include "MyGameInstance.h"
#include "../Base/Managers/UIManager.h"
#include "../Monster/BossMonster.h"
#include "../Monster/AI/AIController_BossMonster.h"
#include "UI/Boss1Widget.h"



AStage1BossGameModeBase::AStage1BossGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ABossMonster> BM(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Monster/BossMonster/BossMonster_BP.BossMonster_BP_C'"));
	if (BM.Succeeded())
	{
		_boss = BM.Class;
	}

	_portalLocation = FVector(-8270.0f, 210.0f, 100.0f);
	_portalRotation = FRotator::ZeroRotator;
	_playerLocation = FVector(-5202.5f, 12.0f, 150.f);
	_playerRotation = FRotator::ZeroRotator;
}

void AStage1BossGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AStage1BossGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AStage1BossGameModeBase::BossClear()
{
	Super::BossClear();
	GAMEINSTANCE->SetStage1Clear(true);
}

void AStage1BossGameModeBase::BossStart()
{
	Super::BossStart();
	UIManager->OpenUI(UI_LIST::Boss);
}

void AStage1BossGameModeBase::SpawnBoss()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("Boss");
	ABossMonster *Boss = GetWorld()->SpawnActor<ABossMonster>(_boss, FVector(-7787.8f, -191.5f, 171.1f), FRotator::ZeroRotator, SpawnParams);
	if (Boss)
	{
		Boss->GetStatComponent()->SetBossLevelInit(1);
		AAIController_BossMonster *BossAI = GetWorld()->SpawnActor<AAIController_BossMonster>(AAIController_BossMonster::StaticClass());
		if (BossAI)
		{
			BossAI->OnPossess(Boss);
		}


		Boss->GetStatComponent()->_PlHPDelegate.AddUObject(UIManager->GetBossUI(), &UBoss1Widget::UpdateBossHPBar);
		Boss->GetStatComponent()->_deathDelegate.AddUObject(this, &AStage1BossGameModeBase::BossClear);
	}
}
