// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/MyGameInstance.h"
#include "Component/StatComponent.h"
#include "Component/InventoryComponent.h"
#include "Item/BaseItem.h"
#include "Item/Equip/EquipItem.h"
#include "Item/Consumes/ConsumeItem.h"
#include "../UI/InventoryWidget.h"
#include "../Item/BaseItem.h"
#include "../Item/Equip/EquipItem.h"
#include "../Component/InventoryComponent.h"
#include "Base/Managers/UIManager.h"
#include "Base/Managers/SoundManager.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> StatData(TEXT("/Script/Engine.DataTable'/Game/Data/StatDataTable.StatDataTable'"));

	if (StatData.Succeeded())
	{
		_statTable = StatData.Object;
	}

	// 현재 에픽 스탯데이터테이블 까지완
	static ConstructorHelpers::FObjectFinder<UDataTable> EpicStatData(TEXT("/Script/Engine.DataTable'/Game/Data/EpicMonsterDataTable.EpicMonsterDataTable'"));

	if (EpicStatData.Succeeded())
	{
		_EpicstatTable = EpicStatData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> BossStatData(TEXT("/Script/Engine.DataTable'/Game/Data/BossMonsterDataTable.BossMonsterDataTable'"));

	if (BossStatData.Succeeded())
	{
		_BossstatTable = BossStatData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ConsumItemData(TEXT("/Script/Engine.DataTable'/Game/Data/Item/ConsumeItemDataTable.ConsumeItemDataTable'"));

	if (ConsumItemData.Succeeded())
	{
		_ConsItemTable = ConsumItemData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EquipItemData(TEXT("/Script/Engine.DataTable'/Game/Data/Item/EquipItemDataTable.EquipItemDataTable'"));

	if (EquipItemData.Succeeded())
	{
		_EquipItemTable = EquipItemData.Object;
	}
}

void UMyGameInstance::SavePlayerStats(class UStatComponent *StatComponent)
{
	if (StatComponent)
	{
		_savedLevel = StatComponent->GetLevel();
		_savedMaxHp = StatComponent->GetMaxHp();
		_savedMaxMp = StatComponent->GetMaxMp();
		_savedStr = StatComponent->GetStr();
		_savedDex = StatComponent->GetDex();
		_savedInt = StatComponent->GetInt();
		_savedCurHp = StatComponent->GetCurHp();
		_savedCurMp = StatComponent->GetCurMp();
		_savedExp = StatComponent->GetExp();
		_savedBonus = StatComponent->GetBonusPoint();
	}
}

void UMyGameInstance::LoadPlayerStats(class UStatComponent *StatComponent)
{
	if (StatComponent)
	{
		StatComponent->SetLevel(_savedLevel);
		StatComponent->SetMaxHp(_savedMaxHp);
		StatComponent->SetMaxMp(_savedMaxMp);
		StatComponent->SetStr(_savedStr);
		StatComponent->SetDex(_savedDex);
		StatComponent->SetInt(_savedInt);
		StatComponent->SetHp(_savedCurHp);
		StatComponent->SetMp(_savedCurMp);
		StatComponent->SetExp(_savedExp);
		StatComponent->SetBonusPoint(_savedBonus);
	}
}

void UMyGameInstance::SaveInventory(class UInventoryComponent *InventoryComponent)
{
	if (InventoryComponent)
	{
		TArray<ABaseItem *> Items = InventoryComponent->GetItemSlots();
		TMap<FString, AEquipItem *> EquipItems = InventoryComponent->GetEquipSlots();

		for (ABaseItem *Item : Items)
		{
			if (Item)
			{
				FItemData ItemData;
				ItemData._Code = Item->GetCode();
				ItemData._Name = Item->GetName();
				ItemData._Type = Item->GetType();
				ItemData._ModTarget = Item->GetModStat();
				ItemData._Description = Item->GetDesc();
				ItemData._Price = Item->GetPrice();
				ItemData._Value = Item->GetValue();
				ItemData._Mesh = Item->GetSkeletalMesh();
				ItemData._Texture = Item->GetTexture();

				SavedInventoryData.Add(ItemData);
			}
		}

		for (auto &Item : EquipItems)
		{
			if (Item.Value)
			{
				FItemData ItemData;
				ItemData._Code = Item.Value->GetCode();
				ItemData._Name = Item.Value->GetName();
				ItemData._Type = Item.Value->GetType();
				ItemData._ModTarget = Item.Value->GetModStat();
				ItemData._Description = Item.Value->GetDesc();
				ItemData._Price = Item.Value->GetPrice();
				ItemData._Value = Item.Value->GetValue();
				ItemData._Mesh = Item.Value->GetSkeletalMesh();
				ItemData._Texture = Item.Value->GetTexture();

				SavedEquipData.Add(Item.Key, ItemData);
			}
		}
	}
}

void UMyGameInstance::LoadInventory(class UInventoryComponent *InventoryComponent)
{
	if (InventoryComponent)
	{
		InventoryComponent->InitSlot();

		for (const FItemData &ItemData : SavedInventoryData)
		{
			ABaseItem *NewItem = nullptr;

			if (ItemData._Type == ItemType::Consume)
			{
				NewItem = GetWorld()->SpawnActor<ABaseItem>(ABaseItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
				NewItem->SetItemWithCode(ItemData._Code);
			}
			else if (ItemData._Type == ItemType::Equipment)
			{
				AEquipItem *EquipItem = GetWorld()->SpawnActor<AEquipItem>(AEquipItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
				EquipItem->SetItemWithCode(ItemData._Code);
				NewItem = EquipItem;
			}

			if (NewItem)
			{
				InventoryComponent->AddItemToSlot(NewItem);
			}
		}

		for (const auto &Item : SavedEquipData)
		{
			AEquipItem *NewItem = nullptr;
			FString EquipType = Item.Key;
			const FItemData &ItemData = Item.Value;

			if (ItemData._Type == ItemType::Equipment)
			{
				AEquipItem* EquipItem = GetWorld()->SpawnActor<AEquipItem>(AEquipItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
				if (EquipItem)
				{
					EquipItem->SetItemWithCode(ItemData._Code);
					NewItem = EquipItem;

                    InventoryComponent->AddItemToEquip(EquipType,NewItem);
				}
			}
		}
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	auto statData = GetStatDataByLevel(1);
	auto EpicData = GetEpicDataByLevel(1);

	InitializeManagers();
}

void UMyGameInstance::InitializeManagers()
{
	FActorSpawnParameters params;
	params.Name = TEXT("UIManager");
	_UIManager = GetWorld()->SpawnActor<AUIManager>(FVector::ZeroVector, FRotator::ZeroRotator, params);

	FActorSpawnParameters soundParams;
	soundParams.Name = TEXT("SoundManager");
	_soundManager = GetWorld()->SpawnActor<ASoundManager>(FVector::ZeroVector, FRotator::ZeroRotator, soundParams);

	FActorSpawnParameters effectParams;
	effectParams.Name = TEXT("EffectManager");
	_effectManager = GetWorld()->SpawnActor<AEffectManager>(FVector::ZeroVector, FRotator::ZeroRotator, effectParams);
}

FMyStatData *UMyGameInstance::GetStatDataByLevel(int level)
{
	auto statData = _statTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return statData;
}

FMyStatData *UMyGameInstance::GetEpicDataByLevel(int level)
{
	auto EpicStatData = _EpicstatTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return EpicStatData;
}

FMyStatData *UMyGameInstance::GetBossDataByLevel(int level)
{
	auto BossStatData = _BossstatTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return BossStatData;
}

FItemData *UMyGameInstance::GetConsumeItemData(int code)
{
	auto ConsumeData = _ConsItemTable->FindRow<FItemData>(*FString::FromInt(code), TEXT(""));
	return ConsumeData;
}

FItemData *UMyGameInstance::GetEquipItemData(int code)
{
	auto EquipData = _EquipItemTable->FindRow<FItemData>(*FString::FromInt(code), TEXT(""));
	return EquipData;
}
