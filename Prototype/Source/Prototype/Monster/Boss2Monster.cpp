// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Boss2Monster.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "AI/AIController_Boss2.h"
#include "../Animation/Monster_Boss2_AnimInstance.h"
#include "BossFireball.h"

ABoss2Monster::ABoss2Monster()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Prime_Helix/Meshes/Prime_Helix.Prime_Helix'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	static ConstructorHelpers::FClassFinder<ABossFireball> BF(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Monster/BossMonster/BossFireball_BP.BossFireball_BP_C'"));
	if (BF.Succeeded())
	{
		_fireball = BF.Class;
	}

	GetCapsuleComponent()->SetCapsuleHalfHeight(300.0f);
	GetCapsuleComponent()->SetCapsuleRadius(100.0f);

	AIControllerClass = AAIController_Boss2::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABoss2Monster::FireballAttack(FVector Location)
{
	float FireballSpacing = 200.0f;
	FVector InitialLocation = GetActorLocation() + FVector(0, 0, 300.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	int FireballCount = 5;
	float MiddleIndex = (FireballCount - 1) / 2.0f;

	TArray<class ABossFireball *> Fireballs;

	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	for (int i = 0; i < FireballCount; i++)
	{

		FVector Offset = RightVector * (i - MiddleIndex) * FireballSpacing;
		FVector SpawnLocation = InitialLocation + Offset;

		ABossFireball *Fireball = GetWorld()->SpawnActor<ABossFireball>(_fireball, SpawnLocation, SpawnRotation);
		if (Fireball)
		{
			Fireballs.Add(Fireball);
		}
	}

	if (Fireballs.IsValidIndex(0))
	{
		Fireballs[0]->LaunchTowards(Location);
	}

	for (int i = 1; i < Fireballs.Num(); i++)
	{
		FTimerHandle TimerHandle;

		GetWorldTimerManager().SetTimer(TimerHandle, [this, Fireballs, i, Location]()
										{
            if (Fireballs.IsValidIndex(i) && Fireballs[i])
            {
                Fireballs[i]->LaunchTowards(Location);
            } }, i * 0.5f, false);
	}
}

void ABoss2Monster::BeginPlay()
{
	Super::BeginPlay();
}

void ABoss2Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_bossMonster02_AnimInstance = Cast<UMonster_Boss2_AnimInstance>(GetMesh()->GetAnimInstance());
	if (_bossMonster02_AnimInstance->IsValidLowLevelFast())
	{
		_bossMonster02_AnimInstance->OnMontageEnded.AddDynamic(this, &ACreature::OnAttackEnded);
		_bossMonster02_AnimInstance->_attackDelegate.AddUObject(this, &ACreature::AttackHit);
		_bossMonster02_AnimInstance->_deathDelegate.AddUObject(this, &AMonster::Disable);
	}
}

void ABoss2Monster::Attack_AI()
{
	if (_isAttacking == false && _bossMonster02_AnimInstance != nullptr)
	{
		_isAttacking = true;
		_bossMonster02_AnimInstance->PlayAttackMontage();
	}

	FireballAttack(FVector::ZeroVector);
}
