// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DragonAnimInstance.h"
#include "GameFramework/Character.h"

UDragonAnimInstance::UDragonAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Dragon
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Animation/Player/Dragon_GroundAttkack_AnimMontage.Dragon_GroundAttkack_AnimMontage'"));

	if (Dragon.Succeeded())
	{
		_myAnimMontage = Dragon.Object;
	}

}

void UDragonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//if (ACharacter* OwningCharacter = Cast<ACharacter>(TryGetPawnOwner()))
	//{
	//	// 현재 속도를 계산하여 _speed 변수 업데이트
	//	_speed = OwningCharacter->GetVelocity().Size();
	//}

}

void UDragonAnimInstance::JumpToSection(int32 sectionIndex)
{
	Super::JumpToSection(sectionIndex);
}

void UDragonAnimInstance::PlayAttackMontage()
{
	Super::PlayAttackMontage();
}


void UDragonAnimInstance::AnimNotify_Attackhit()
{
	_attackDelegate_Dragon.Broadcast();
}

void UDragonAnimInstance::AnimNotify_Death()
{
	_deathDelegate_Dragon.Broadcast();
}
