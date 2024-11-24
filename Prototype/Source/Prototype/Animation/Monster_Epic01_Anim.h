// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BaseAnimInstance.h"
#include "Monster_Epic01_Anim.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UMonster_Epic01_Anim : public UBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UMonster_Epic01_Anim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void PlayAttackMontage()override;

	virtual void JumpToSection(int32 sectionIndex) override;


	virtual void PlayAttackFarMontage() override;

	virtual void PlayAttackDotrMontage() override;

	AttackDelegate _attackDelegate;

	DeathDelegate _death_Epic_MonsterDelegate;

	UFUNCTION()
	virtual void AnimNotify_Attackhit();

	UFUNCTION()
	virtual void AnimNotify_Death();



};
