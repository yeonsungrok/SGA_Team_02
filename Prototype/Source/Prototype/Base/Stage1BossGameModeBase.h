// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossGameModeBase.h"
#include "Stage1BossGameModeBase.generated.h"

/**
 *
 *
 */

UCLASS()
class PROTOTYPE_API AStage1BossGameModeBase : public ABossGameModeBase
{
	GENERATED_BODY()

public:
	AStage1BossGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void BossClear();

	void BossStart();

	virtual void SpawnBoss() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABossMonster> _boss;
};
