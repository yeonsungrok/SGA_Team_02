// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/BossGameModeBase.h"
#include "MyGameInstance.h"
#include "../Base/Managers/UIManager.h"
#include "../Player/MyPlayer.h"
#include "../Player/MyPlayerController.h"
#include "../Player/Portal/Portal_Home.h"
#include "../Component/StatComponent.h"
#include "../Component/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"


ABossGameModeBase::ABossGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APortal_Home> PH(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Player/Portal/HomePortal_BP.HomePortal_BP_C'"));
	if (PH.Succeeded())
	{
		_portal = PH.Class;
	}
}

void ABossGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (Player)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
																				  {
            if (UIManager && UIManager->GetSkillUI())
            {
                UIManager->GetSkillUI()->LockAllSkill();
            } }),
										0.1f, false);
	}
}

void ABossGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABossGameModeBase::BossClear()
{
	if (_portal)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = TEXT("PortalHome");

		APortal_Home *Portal = GetWorld()->SpawnActor<APortal_Home>(_portal, _portalLocation, _portalRotation, SpawnParams);
	}
}

void ABossGameModeBase::BossStart()
{
	AMyPlayer *player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
		UIManager->GetSkillUI()->UnLockAllSkill();
		player->SetActorLocationAndRotation(_playerLocation, _playerRotation);
	}

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SpawnBoss();
}
