// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/MyGameModeBase.h"
#include "NormalGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ANormalGameModeBase : public AMyGameModeBase
{
	GENERATED_BODY()
public:
	ANormalGameModeBase();

	virtual void BeginPlay() override;

	void SpawnMonster(FVector BaseLocation,int size);

	void InitializeMonsterPool();

	void RespawnMonster(class ANormalMonster* Monster);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ANormalMonster> _monster;

	UPROPERTY()
	TArray<ANormalMonster*> _monsterPool;
	
};
