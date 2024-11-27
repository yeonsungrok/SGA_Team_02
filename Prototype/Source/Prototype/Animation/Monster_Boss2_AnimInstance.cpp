// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Monster_Boss2_AnimInstance.h"


UMonster_Boss2_AnimInstance::UMonster_Boss2_AnimInstance()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> BM_01
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Animation/Monster/BossMonster/Boss2/Boss2Attack_Montage.Boss2Attack_Montage'"));

	if (BM_01.Succeeded())
	{
		_myAnimMontage = BM_01.Object;
	}
   
}


void UMonster_Boss2_AnimInstance::AnimNotify_Attackhit()
{
    _attackDelegate.Broadcast();
}

void UMonster_Boss2_AnimInstance::AnimNotify_Death()
{
    _deathDelegate.Broadcast();
}