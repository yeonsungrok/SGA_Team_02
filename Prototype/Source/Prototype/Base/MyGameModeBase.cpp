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
		UMyGameInstance *GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			if (GameInstance->GetFirst())
			{
				UE_LOG(LogTemp,Warning,TEXT("First GamemOde"));
				UStatComponent *StatComponent = player->FindComponentByClass<UStatComponent>();
				UInventoryComponent *InvenComponent = player->FindComponentByClass<UInventoryComponent>();
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
				GAMEINSTANCE->InitializeManagers();
				
				UStatComponent *StatComponent = player->FindComponentByClass<UStatComponent>();
				UInventoryComponent *InvenComponent = player->FindComponentByClass<UInventoryComponent>();
				if(StatComponent)
				{
					GameInstance->LoadPlayerStats(StatComponent);
				}
				if(InvenComponent)
				{
					GameInstance->LoadInventory(InvenComponent);
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
