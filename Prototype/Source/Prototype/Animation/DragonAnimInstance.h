// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BaseAnimInstance.h"
#include "DragonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UDragonAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	

public:
	UDragonAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void JumpToSection(int32 sectionIndex) override;
	/*void SetSpeed(float NewSpeed);*/

	AttackDelegate _attackDelegate;
	DeathDelegate _deathDelegate_Dragon;

	UFUNCTION()
	virtual void AnimNotify_Death();

	//추가시도...
	void SetJumping(bool bJumping) { bIsJumping = bJumping; }
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsJumping;

};
