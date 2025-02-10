// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Stage2NormalGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API AStage2NormalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStage2NormalGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	void SpawnMonster(FVector BaseLocation,int size);

	void InitializeMonsterPool();

	void RespawnMonster(class ANormalMonster* Monster);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ANormalMonster> _monster;

	UPROPERTY()
	TArray<ANormalMonster*> _monsterPool;
};
