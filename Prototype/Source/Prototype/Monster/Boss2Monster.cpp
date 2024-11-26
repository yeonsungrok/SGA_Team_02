// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Boss2Monster.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
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
}

void ABoss2Monster::FireballAttack(FVector Location)
{
	float FireballSpacing = 200.0f;
	FVector InitialLocation = GetActorLocation() + FVector(0, 0, 500.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	int FireballCount = 5;
	float MiddleIndex = (FireballCount - 1) / 2.0f;


		TArray<class ABossFireball*> Fireballs;

	for (int i = 0; i < FireballCount; i++)
	{
		FVector SpawnLocation = InitialLocation + FVector(0, (i - MiddleIndex) * FireballSpacing, 0);
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

	FireballAttack(FVector::ZeroVector);
}

void ABoss2Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
