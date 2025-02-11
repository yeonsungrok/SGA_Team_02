// Fill out your copyright notice in the Description page of Project Settings.

#include "HomeGameModebase.h"
#include "../Component/StatComponent.h"
#include "MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "../Player/MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../Player/MyPlayer.h"
#include "../Player/Portal/Portal_Stage2_Normal.h"
#include "../Monster/NormalMonster.h"
#include "../Monster/AI/AIController_NormalMonster.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Kismet/GameplayStatics.h"

AHomeGameModebase::AHomeGameModebase()
{
	static ConstructorHelpers::FClassFinder<APortal_Stage2_Normal> PS(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Player/Portal/Stage2Normal_Portal_BP.Stage2Normal_Portal_BP_C'"));
	if (PS.Succeeded())
	{
		_portal2 = PS.Class;
	}
}

void AHomeGameModebase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
																			  {
            if (UIManager && UIManager->GetSkillUI())
            {
                UIManager->GetSkillUI()->LockAllSkill();
            } }),
									0.1f, false);

	if (GameInstance->GetStage1Clear())
	{
		if (_portal2)
		{
			FVector Location(5690.f, 5900.f, -40.f);
			FRotator Rotation(0.f, 0.f, 0.f);
			GetWorld()->SpawnActor<APortal_Stage2_Normal>(_portal2, Location, Rotation);
		}
	}

	Player->GetStatComponent()->Reset();
}

void AHomeGameModebase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
