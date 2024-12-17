// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Creature.h"

#include "Creature.h"
#include "../Animation/DragonAnimInstance.h"
#include "../Player/MyPlayer.h"


#include "Dragon.generated.h"

/**
 * 
 */
class AMyPlayer;

UCLASS()
class PROTOTYPE_API ADragon : public ACreature
{
	GENERATED_BODY()

public:
    ADragon();

protected:
    virtual void BeginPlay() override;

 
public:
    void TransformToHuman();
    void TransformToDragon();

    void ToggleTransformation();

    void OnMonsterHit(class AMonster* HitMonster, const FHitResult& Hit);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;
    virtual void Tick(float DeltaTime) override;

    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

    UStatComponent* GetStatComponent() { return _StatCom; }


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* _Change;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* _lookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* _moveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* _jumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* _attackAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* _springArm;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* _camera;

private:

    void Move(const FInputActionValue& value);
    void Look(const FInputActionValue& value);
    void JumpA(const FInputActionValue& value);
    void AttackA(const FInputActionValue& value);


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    class UDragonAnimInstance* _dragonAnimInstance;
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TSubclassOf<UAnimInstance> DragonAnimInstanceClass;

    class AMyPlayer* _knightInstance;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
    class UStatWidget* _statWidget;

};
	

