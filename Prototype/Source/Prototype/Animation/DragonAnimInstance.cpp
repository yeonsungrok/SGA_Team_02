// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DragonAnimInstance.h"
#include "GameFramework/Character.h"

UDragonAnimInstance::UDragonAnimInstance()
{

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

//void UDragonAnimInstance::SetSpeed(float NewSpeed)
//{
//	_speed = NewSpeed;
//}

void UDragonAnimInstance::AnimNotify_Death()
{
	_deathDelegate_Dragon.Broadcast();
}
