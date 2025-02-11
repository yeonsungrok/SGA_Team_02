// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/NormalGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Monster/NormalMonster.h"
#include "../Monster/AI/AIController_NormalMonster.h"


ANormalGameModeBase::ANormalGameModeBase()
{
    static ConstructorHelpers::FClassFinder<ANormalMonster> NM(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Monster/NormalMonster/BaseMap_NormalMonster_03_BP.BaseMap_NormalMonster_03_BP_C'"));
	if (NM.Succeeded())
	{
		_monster = NM.Class;
	}
}

void ANormalGameModeBase::BeginPlay()
{
    Super::BeginPlay();
	InitializeMonsterPool();
}

void ANormalGameModeBase::SpawnMonster(FVector BaseLocation,int size)
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

		float SpawnRadius = 500.0f;

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

void ANormalGameModeBase::InitializeMonsterPool()
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

void ANormalGameModeBase::RespawnMonster(class ANormalMonster* Monster)
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
