// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/Stage2NormalGameModeBase.h"
#include "Component/StatComponent.h"
#include "Component/InventoryComponent.h"
#include "../Player/Portal/Portal_Home.h"
#include "MyGameInstance.h"
#include "../Player/MyPlayer.h"
#include "Base/Managers/UIManager.h"
#include "../Monster/NormalMonster.h"
#include "../Monster/AI/AIController_NormalMonster.h"
#include "UI/Boss1Widget.h"
#include "Kismet/GameplayStatics.h"

AStage2NormalGameModeBase::AStage2NormalGameModeBase()
{
}

void AStage2NormalGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeMonsterPool();

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

			APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetInputMode(FInputModeGameOnly());
			}
		}
	}

	SpawnMonster(FVector(-1350.f,856.0f,209.0f),10);
	SpawnMonster(FVector(680.f,-1331.0f,1156.0f),10);
	
}

void AStage2NormalGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AStage2NormalGameModeBase::SpawnMonster(FVector BaseLocation,int size)
{
	if (_monsterPool.Num() == 0) return;

	for (int i = 0; i < size; ++i)
	{
		ANormalMonster* AvailableMonster = nullptr;
		for (ANormalMonster* Monster : _monsterPool)
		{
			if (Monster->IsHidden())
			{
				AvailableMonster = Monster;
				break;
			}
		}
		if (!AvailableMonster) return;

		float SpawnRadius = 300.0f;

	  	FVector2D RandOffset = FMath::RandPointInCircle(SpawnRadius);
        FVector SpawnLocation = BaseLocation + FVector(RandOffset.X, RandOffset.Y, 0.0f);

        AvailableMonster->SetActorLocation(SpawnLocation);
        AvailableMonster->SetActorRotation(FRotator::ZeroRotator);
        AvailableMonster->SetActorHiddenInGame(false);
        AvailableMonster->SetActorEnableCollision(true);
        AvailableMonster->GetStatComponent()->SetMonsterLevelInit(1);

        AAIController_NormalMonster* MonsterAI = GetWorld()->SpawnActor<AAIController_NormalMonster>(AAIController_NormalMonster::StaticClass());
        if (MonsterAI)
        {
            MonsterAI->OnPossess(AvailableMonster);
        }
	}
}

void AStage2NormalGameModeBase::InitializeMonsterPool()
{
	if (!_monster) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < 20; ++i)
	{
		ANormalMonster* Monster = GetWorld()->SpawnActor<ANormalMonster>(_monster, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (Monster)
		{
			Monster->SetActorHiddenInGame(true);
			Monster->SetActorEnableCollision(false);
			Monster->SetLifeSpan(0.0f);
			_monsterPool.Add(Monster);
		}
	}
}

void AStage2NormalGameModeBase::RespawnMonster(class ANormalMonster* Monster)
{
	 if (Monster)
    {
        Monster->SetActorHiddenInGame(true);
        Monster->SetActorEnableCollision(false);

        FTimerHandle RespawnTimerHandle;
        GetWorldTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateLambda([this, Monster]()
        {
			float SpawnRadius = 300.0f;
			FVector RespawnLocation = FVector(-44.f,-6230.0f,-15.0f);
	  		FVector2D RandOffset = FMath::RandPointInCircle(SpawnRadius);
        	FVector SpawnLocation = RespawnLocation + FVector(RandOffset.X, RandOffset.Y, 0.0f);
          
            Monster->SetActorLocation(SpawnLocation);
            Monster->SetActorRotation(FRotator::ZeroRotator);
            Monster->SetActorHiddenInGame(false);
            Monster->SetActorEnableCollision(true);
            Monster->GetStatComponent()->SetMonsterLevelInit(1);

            AAIController_NormalMonster* MonsterAI = GetWorld()->SpawnActor<AAIController_NormalMonster>(AAIController_NormalMonster::StaticClass());
            if (MonsterAI)
            {
                MonsterAI->OnPossess(Monster);
            }

        }), 1.0f, false);
    }
}
