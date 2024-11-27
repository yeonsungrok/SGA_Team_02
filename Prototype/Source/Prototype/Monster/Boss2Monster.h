// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Boss2Monster.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ABoss2Monster : public AMonster
{
	GENERATED_BODY()
public:
	ABoss2Monster();

	void FireballAttack(FVector Location);

private:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Attack_AI() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UMonster_Boss2_AnimInstance* _bossMonster02_AnimInstance;

	TSubclassOf<class ABossFireball> _fireball;	
};
