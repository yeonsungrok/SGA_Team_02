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
    AMyPlayer* player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if(GameInstance)
		{
			UStatComponent* StatComponent = player->FindComponentByClass<UStatComponent>();
			UInventoryComponent* InvenComponent = player->FindComponentByClass<UInventoryComponent>();
			if (StatComponent)
			{
				if(GameInstance->GetFirst())
				{
					player->_StatCom->SetLevelInit(1);
					player->_inventoryComponent->InitSlot();
					GameInstance->SetFirst(false);
				}
				else
				{
					GameInstance->LoadInventory(InvenComponent);
					GameInstance->LoadPlayerStats(StatComponent);
				}
				
			}
		}
		
	}
}

void AMyGameModeBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}
