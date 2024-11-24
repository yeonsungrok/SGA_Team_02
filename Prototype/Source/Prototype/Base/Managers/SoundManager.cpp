// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/Managers/SoundManager.h"
#include "../Prototype/VFX/SoundEffect.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASoundManager::ASoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_rootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	_rootComponent->SetupAttachment(RootComponent);
	// Player Swing Sword Sound
	CreateSound("SwingSound_Sword_01", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/PlayerSwingSound_BP.PlayerSwingSound_BP_C'");
	// Player AttackHit Sound
	CreateSound("TakeSound_Sword_01", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/PlayerAtkHitSound_BP.PlayerAtkHitSound_BP_C'");
	// Player GuardShield On / Off
	CreateSound("ShieldGuard_Off", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/PlayerGuardOff_BP.PlayerGuardOff_BP_C'");
	CreateSound("ShieldGuard_On", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/PlayerGuardOn_BP.PlayerGuardOn_BP_C'");

	// Player Skill Sound
	CreateSound("Skill01_Sound", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_01_BP.SkillSound_01_BP_C'");
	CreateSound("Skill02_Sound", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_02_BP.SkillSound_02_BP_C'");
	CreateSound("Skill03_Sound_Begin_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_03_Begin_BP.SkillSound_03_Begin_BP_C'");
	CreateSound("Skill03_Sound_Hit_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_03_Hit_BP.SkillSound_03_Hit_BP_C'");
	CreateSound("Skill03_Shout","/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_03_Shout_BP.SkillSound_03_Shout_BP_C'");
	CreateSound("Skill04_Sound_Start", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_04_Begin_BP.SkillSound_04_Begin_BP_C'");
	CreateSound("Skill04_Sound_02_during", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SkillSound_04_Durring_BP.SkillSound_04_Durring_BP_C'");

	// NormalMonster Sound
	CreateSound("NormalMonster_AttackSound", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_N_AttackSound_BP.Monster_N_AttackSound_BP_C'");
	CreateSound("NormalMonster_TakeDamageSound", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_N_TakeDamageSound_BP.Monster_N_TakeDamageSound_BP_C'");
	CreateSound("NormalMonster_DeathSound", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_N_DeathSound_BP.Monster_N_DeathSound_BP_C'");

	// BossMonster Sound
	CreateSound("BossMonsterAttack_Hard", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_Boss_AttackSound_BP.Monster_Boss_AttackSound_BP_C'");
	CreateSound("BossMonsterPunch", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_Boss_AttackSoundPunch_BP.Monster_Boss_AttackSoundPunch_BP_C'");

	// EpicMonster Sound
	CreateSound("EpicMonsterAttack_Far_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_Epic_AttackFireBall.Monster_Epic_AttackFireBall_C'");
	CreateSound("EpicMonsterAttack_MagicDot_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_Epic_AttackMagicDot.Monster_Epic_AttackMagicDot_C'");
	CreateSound("Morigesh_Effort_Death_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/Monster_Epic_Death.Monster_Epic_Death_C'");
	CreateSound("SpawnSound_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/SpawnSound_BP.SpawnSound_BP_C'");


	//UI Base Sound
	CreateSound("BaseUISound_02_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/BaseUISound_02_BP.BaseUISound_02_BP_C'");
	//Level Up Sound
	CreateSound("LevelupSound_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/LevelupSound_BP.LevelupSound_BP_C'");

	CreateSound("ObstacleBroken_Cue", "/Script/Engine.Blueprint'/Game/Blueprint/Sound/ObstacleBroken_BP.ObstacleBroken_BP_C'");
}

void ASoundManager::BeginPlay()
{
	Super::BeginPlay();

	Destroy();
	CreateSoundEffect();
}

void ASoundManager::Destroy()
{
	for (auto soundEffectPair : _soundEffectTable)
    {
        for (auto soundEffect : soundEffectPair.Value)
        {
            soundEffect->Destroy();
        }
    }

    _soundEffectTable.Empty();
}

void ASoundManager::PlaySound(FString name, FVector location)
{
	if (_soundEffectTable.Contains(name) == false)
		return;

	auto findSound = _soundEffectTable[name].FindByPredicate(
		[](ASoundEffect* soundEffect)-> bool
		{
			if (soundEffect->IsPlaying())
				return false;
			return true;
		});

	if (findSound)
		(*findSound)->Play(location);

}

void ASoundManager::PlaySoundWithDuration(FString name, FVector location, float duration)
{

	if (!_soundEffectTable.Contains(name))
		return;

	auto findSound = _soundEffectTable[name].FindByPredicate(
		[](ASoundEffect* soundEffect)-> bool
		{
			return !soundEffect->IsPlaying();  // 현재 재생 중이 아닌 소리만 찾기
		});

	if (findSound)
	{
		ASoundEffect* soundEffect = *findSound;
		soundEffect->Play(location);

		// 소리가 재생된 후, 지정된 duration 시간이 지나면 soundEffect의 Stop 함수를 호출하도록 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			SoundDurationTimerHandle,
			FTimerDelegate::CreateLambda([soundEffect]() {
				soundEffect->Stop();
				}),
			duration,
			false
		);
	}
}






void ASoundManager::CreateSound(FString name, FString path)
{
	ConstructorHelpers::FClassFinder<ASoundEffect> soundEffect(*path);
	if (soundEffect.Succeeded())
	{
		_soundTable.Add(name, soundEffect.Class);
	}

}

// Called every frame
void ASoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoundManager::CreateSoundEffect()
{
	for (auto classPair : _soundTable)
	{
		FString name = classPair.Key;

		_soundEffectTable.Add(name);
		for (int32 i = 0; i < _poolCount; i++)
		{
			FString tempName = name + FString::FromInt(i);
			FActorSpawnParameters parameters;
			parameters.Name = FName(*tempName);
			auto soundEffect = GetWorld()->SpawnActor<ASoundEffect>(classPair.Value, FVector::ZeroVector, FRotator::ZeroRotator, parameters);

			soundEffect->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			_soundEffectTable[name].Add(soundEffect);
		}
	}

}

