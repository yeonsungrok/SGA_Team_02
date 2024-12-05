// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopWidget.h"
#include "Base/MyGameInstance.h"
#include "Player/MyPlayer.h"
#include "Components/UniformGridPanel.h"
#include "InventoryWidget.h"
#include "Component/InventoryComponent.h"
#include "Component/ShopComponent.h"
#include "Components/CanvasPanel.h"
#include "UI/Elements/IndexedButton.h"
#include "Components/TextBlock.h"

bool UShopWidget::Initialize()
{
	bool result = Super::Initialize();

	SetInvenButtons();

	_SellPanels.Add(SellPanel_0);
	_SellPanels.Add(SellPanel_1);
	_SellPanels.Add(SellPanel_2);
	_SellPanels.Add(SellPanel_3);
	_SellPanels.Add(SellPanel_4);

	Cast<UButton>(_SellPanels[0]->GetChildAt(4))->OnClicked.AddDynamic(this, &UShopWidget::BuySlot1);
	Cast<UButton>(_SellPanels[1]->GetChildAt(4))->OnClicked.AddDynamic(this, &UShopWidget::BuySlot2);
	Cast<UButton>(_SellPanels[2]->GetChildAt(4))->OnClicked.AddDynamic(this, &UShopWidget::BuySlot3);
	Cast<UButton>(_SellPanels[3]->GetChildAt(4))->OnClicked.AddDynamic(this, &UShopWidget::BuySlot4);
	Cast<UButton>(_SellPanels[4]->GetChildAt(4))->OnClicked.AddDynamic(this, &UShopWidget::BuySlot5);

	return result;
}

void UShopWidget::UpdateShopList(TArray<ABaseItem*> list)
{
	for (int i = 0; i < SHOP_LIST_MAX; i++)
	{
		auto item = Cast<UIndexedButton>(_SellPanels[i]->GetChildAt(0));
		item->SetItem(list[i]);
		item->ButtonUpdate();
		auto name = Cast<UTextBlock>(_SellPanels[i]->GetChildAt(1));
		name->SetText(FText::FromString(list[i]->GetName()));
		auto price = Cast<UTextBlock>(_SellPanels[i]->GetChildAt(2));
		price->SetText(FText::FromString(FString::FromInt(list[i]->GetPrice()) + FString(TEXT(" G"))));
		auto description = Cast<UTextBlock>(_SellPanels[i]->GetChildAt(3));
		description->SetText(FText::FromString(list[i]->GetDesc()));
	}
}

void UShopWidget::ReflectInvenSlots(AMyPlayer* player)
{
	_isSlotFull = true;
	TArray<ABaseItem*> itemList = player->_inventoryComponent->GetItemSlots();
	for (int i = 0; i < Button_.Num(); i++)
	{
		Button_[i]->SetItem(itemList[i]);
		Button_[i]->ButtonUpdate();
		if (_isSlotFull && itemList[i] == nullptr)
			_isSlotFull = false;
	}

	//TODO : Get Wallet too & Call this every opening
	int32 Gold = player->_inventoryComponent->GetHowMuchIHave();
	p_gold = Gold;
	GoldAmount->SetText(FText::FromString(FString::FromInt(Gold)));
}

void UShopWidget::SetInvenButtons()
{
	TArray<UWidget*> widgets;
	widgets = ItemSlots->GetAllChildren();

	int32 index = 0;
	for (UWidget* widget : widgets)
	{
		UIndexedButton* button = Cast<UIndexedButton>(widget);
		if (button)
		{
			button->SetIndex(index);
			button->SetSlotType(SlotType::Inventory);
			button->SetIsEnabled(true);
			Button_.Add(button);

			index++;
		}
	}
}

DealContext UShopWidget::BuyResult(int32 slot)
{
	if (slot >= _SellPanels.Num())
		return DealContext::Error;

	auto item = Cast<UIndexedButton>(_SellPanels[slot]->GetChildAt(0))->GetItem();

	if (item == nullptr)
		return DealContext::EmptySlot;
	if (p_gold < item->GetPrice())
		return DealContext::MoneyNotEnough;
	if (_isSlotFull)
		return DealContext::InventoryIsFull;

	return DealContext::Succeed;
}

void UShopWidget::TryBuyItem(int32 slot)
{
	auto result = BuyResult(slot);
	switch (result)
	{
	case DealContext::Error:
		UE_LOG(LogTemp, Error, TEXT("Item Buy Error!"));
		break;
	case DealContext::Succeed:
		SaleSucceed.Broadcast(slot);
		break;
	case DealContext::MoneyNotEnough:
		UE_LOG(LogTemp, Warning, TEXT("Doenst have enough money"));
		break;
	case DealContext::InventoryIsFull:
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!!"));
		break;
	case DealContext::EmptySlot:
		UE_LOG(LogTemp, Warning, TEXT("You cant buy Air"));
		break;
	default:
		break;
	}
}

void UShopWidget::BuySlot1()
{
	TryBuyItem(0);
}

void UShopWidget::BuySlot2()
{
	TryBuyItem(1);
}

void UShopWidget::BuySlot3()
{
	TryBuyItem(2);
}

void UShopWidget::BuySlot4()
{
	TryBuyItem(3);
}

void UShopWidget::BuySlot5()
{
	TryBuyItem(4);
}
