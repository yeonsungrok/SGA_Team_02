// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShopComponent.h"
#include "Base/MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "UI/ShopWidget.h"

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
	
}

void UShopComponent::SetCustomer(AMyPlayer* target)
{
	_customer = target;
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

void UShopComponent::Sale(int32 index)
{
}

