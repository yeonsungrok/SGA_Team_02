// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShopComponent.h"
#include "Base/MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "UI/ShopWidget.h"
#include "Player/MyPlayer.h"

// Sets default values for this component's properties
UShopComponent::UShopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UShopComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetSales();
}

void UShopComponent::SetCustomer(AMyPlayer* target)
{
	_customer = target;
}

void UShopComponent::Init()
{
	SetSales();
}

void UShopComponent::SetSales()
{
	auto data = GAMEINSTANCE->GetSellingData();
	ABaseItem* item;
	for (auto it : data)
	{
		item = GAMEINSTANCE->SellDataToItemData(it);
		_sallings.Add(item);
	}

	UIManager->GetShopUI()->UpdateShopList(_sallings);
}

DealContext UShopComponent::Sale(int32 index)
{
	//TODO : void형 함수로 바꾸기(shop ui가 component를 모를 수 있게)
	//		 DealContext처리를 shop ui 내부 함수에서 처리하기
	//		 ==> 이 함수에서는 DealContext::Succeed만 처리하게
	if (_customer == nullptr)
		return DealContext::Error;

	if (index >= SHOP_LIST_MAX)
		return DealContext::Error;

	if (_sallings[index] == nullptr)
		return DealContext::Error;

	auto p_inventory = _customer->_inventoryComponent;

	int32 p_wallet = p_inventory->GetHowMuchIHave();
	if (p_wallet < _sallings[index]->GetPrice())
		return DealContext::MoneyNotEnough;

	if (p_inventory->IsSlotFull())
		return DealContext::InventoryIsFull;

	ABaseItem* merch = nullptr;
	if (_sallings[index]->GetType() == ItemType::Consume)
	{
		merch = GetWorld()->SpawnActor<ABaseItem>(ABaseItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		merch->SetItemWithCode(_sallings[index]->GetCode());
	}
	else if (_sallings[index]->GetType() == ItemType::Equipment)
	{
		AEquipItem* EquipItem = GetWorld()->SpawnActor<AEquipItem>(AEquipItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		EquipItem->SetItemWithCode(_sallings[index]->GetCode());
		EquipItem->SetEquipType(_sallings[index]->GetEquip());
		merch = EquipItem;
	}
	p_inventory->GettingMoney(-merch->GetPrice());
	_customer->_inventoryComponent->AddItem(0, merch);
	return DealContext::Succeed;
}

