// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "../Component/StatComponent.h"
#include "MyGameInstance.h"
#include "../Player/MyPlayer.h"
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{
}

void AMyGameModeBase::BeginPlay()
{
	AMyPlayer *player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		UStatComponent *StatComponent = player->FindComponentByClass<UStatComponent>();
		UInventoryComponent *InvenComponent = player->FindComponentByClass<UInventoryComponent>();
		UMyGameInstance *GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			if (GameInstance->GetFirst())
			{
				UE_LOG(LogTemp,Warning,TEXT("First GamemOde"));
				
				if(StatComponent)
				{
					player->_StatCom->SetLevelInit(1);
				}
				if(InvenComponent)
				{
					player->_inventoryComponent->InitSlot();
				}
				GameInstance->SetFirst(false);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("no first GamemOde"));
				GameInstance->InitializeManagers();
				
				if(StatComponent)
				{
					GameInstance->LoadPlayerStats(StatComponent);
				}
				if(InvenComponent)
				{
					player->_inventoryComponent->InitSlot();
				}
				GameInstance->LoadPlayerSkeletal(player);
			}
		}
	}
}

void AMyGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
