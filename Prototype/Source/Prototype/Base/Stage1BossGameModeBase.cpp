// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/Stage1BossGameModeBase.h"
#include "../Component/StatComponent.h"
#include "../Component/InventoryComponent.h"
#include "MyGameInstance.h"
#include "../Player/MyPlayer.h"
#include "../Base/Managers/UIManager.h"
#include "../Monster/BossMonster.h"
#include "../Monster/AI/AIController_BossMonster.h"
#include "UI/Boss1Widget.h"
#include "Kismet/GameplayStatics.h"




AStage1BossGameModeBase::AStage1BossGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ABossMonster> BM(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Monster/BossMonster/BossMonster_BP.BossMonster_BP_C'"));
    if (BM.Succeeded())
    {
        _boss = BM.Class;
    }

}

void AStage1BossGameModeBase::BeginPlay()
{
	AMyPlayer *player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		UMyGameInstance *GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->InitializeManagers();
			UStatComponent *StatComponent = player->FindComponentByClass<UStatComponent>();
			if (StatComponent)
			{
				GameInstance->LoadPlayerStats(StatComponent);
			}

			UInventoryComponent *InvenComponent = player->FindComponentByClass<UInventoryComponent>();
			if (InvenComponent)
			{
				GameInstance->LoadInventory(InvenComponent);
			}
			GameInstance->LoadPlayerSkeletal(player);
		}
	}

	FActorSpawnParameters SpawnParams;
    SpawnParams.Name = TEXT("Boss");

	ABossMonster* Boss = GetWorld()->SpawnActor<ABossMonster>(_boss, FVector(-7935.8f,-264.5f,174.1f), FRotator::ZeroRotator, SpawnParams);
	if(Boss)
	{
		AAIController_BossMonster* BossAI = GetWorld()->SpawnActor<AAIController_BossMonster>(AAIController_BossMonster::StaticClass());
        if (BossAI)
        {
            BossAI->OnPossess(Boss);
        }

		UIManager->OpenUI(UI_LIST::Boss);

		Boss->_StatCom->_PlHPDelegate.AddUObject(UIManager->GetBossUI(), &UBoss1Widget::UpdateBossHPBar);
	}

}


void AStage1BossGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}




