// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
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

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APortal_Stage2_Normal> PS(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Player/Portal/Stage2Normal_Portal_BP.Stage2Normal_Portal_BP_C'"));
	if (PS.Succeeded())
	{
		_portal2 = PS.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Dragon_UI.Dragon_UI_C'"));
	if (UW.Succeeded())
	{
		_dragonUI = UW.Class;
	}
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	AMyPlayer *player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		UStatComponent *StatComponent = player->FindComponentByClass<UStatComponent>();
		UInventoryComponent *InvenComponent = player->FindComponentByClass<UInventoryComponent>();
		UMyGameInstance *GameInstance = Cast<UMyGameInstance>(GetGameInstance());

		APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
		FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            if (UIManager && UIManager->GetSkillUI())
            {
                UIManager->GetSkillUI()->LockAllSkill();
            }
        }), 0.1f, false);

		if (GameInstance)
		{
			if (GameInstance->GetFirst())
			{
				GameInstance->InitializeManagers();

				if (StatComponent)
				{
					player->GetStatComponent()->SetLevelInit(1);
				}
				if (InvenComponent)
				{
					player->GetInventory()->InitSlot();
				}

				GameInstance->SetFirst(false);
			}
			else
			{
				GameInstance->InitializeManagers();

				if (StatComponent)
				{
					GameInstance->LoadPlayerStats(StatComponent);
				}
				if (InvenComponent)
				{
					GameInstance->LoadInventory(InvenComponent);
				}
				GameInstance->LoadPlayerSkeletal(player);
				player->GetStatComponent()->Reset();

				if (GameInstance->GetStage1Clear())
				{
					if (_portal2)
					{
						FVector Location(5690.f, 5900.f, -40.f);
						FRotator Rotation(0.f, 0.f, 0.f);
						GetWorld()->SpawnActor<APortal_Stage2_Normal>(_portal2, Location, Rotation);
					}
				}
			}
		}
	}
}

void AMyGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


