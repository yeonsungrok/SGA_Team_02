// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopWidget.h"
#include "InventoryWidget.h"
#include "Component/ShopComponent.h"
#include "Components/CanvasPanel.h"
#include "UI/Elements/IndexedButton.h"
#include "Components/TextBlock.h"

bool UShopWidget::Initialize()
{
	bool result = Super::Initialize();

	_SellPanels.Add(SellPanel_0);
	_SellPanels.Add(SellPanel_1);
	_SellPanels.Add(SellPanel_2);
	_SellPanels.Add(SellPanel_3);
	_SellPanels.Add(SellPanel_4);

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
