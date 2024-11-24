// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatWidget.h"


#include "Base/MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "UI/InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Player/MyPlayer.h"
#include "Components/TextBlock.h"



void UStatWidget::NativeConstruct()
{
	
	Super::NativeConstruct();

	auto array = SlotGrid->GetAllChildren();

	for (auto widget : array)
	{
		auto btn = Cast<UButton>(widget);
		if (btn)
		{
			_slotBtns.Add(btn);
			_slotBtnText.Add(Cast<UTextBlock>(btn->GetChildAt(0)));
		}
	}

	if (HPUp)
	{
		HPUp->OnClicked.AddDynamic(this, &UStatWidget::HPUpClick);
	}

	if (MPUp)
	{
		MPUp->OnClicked.AddDynamic(this, &UStatWidget::MpUpClick);
	}

	if (STRUp)
	{
		STRUp->OnClicked.AddDynamic(this, &UStatWidget::STRUpClick);

	}
	if (DEXUp)
	{
		DEXUp->OnClicked.AddDynamic(this, &UStatWidget::DEXUpClick);
	}
	if (INTUp)
	{
		INTUp->OnClicked.AddDynamic(this, &UStatWidget::INTUpClick);
	}

	if (HPDown)
	{
		HPDown->OnClicked.AddDynamic(this, &UStatWidget::HPDownClick);
	}
	
	if (MPDown)
	{
		MPDown->OnClicked.AddDynamic(this, &UStatWidget::MPDownClick);
	}

	if (STRDown)
	{
		STRDown->OnClicked.AddDynamic(this, &UStatWidget::STRDownClick);
	}

	if (DEXDown)
	{
		DEXDown->OnClicked.AddDynamic(this, &UStatWidget::DEXDownClick);
	}

	if (INTDown)
	{
		INTDown->OnClicked.AddDynamic(this, &UStatWidget::INTDownClick);
	}
}

void UStatWidget::StatUpdate(StatType type, int amount)
{
	AMyPlayer* player = Cast<AMyPlayer>(GetOwningPlayerPawn());

	if (player && player->_StatCom)
	{
		int32 currentStat = 0;
		int32 baseStat = player->_StatCom->GetBaseStat(type);
		int32 bonusPoints = player->_StatCom->GetBonusPoint();

		// 현재 스탯 값을 가져오기
		switch (type)
		{
		case StatType::HP:
			currentStat = player->_StatCom->GetMaxHp();
			break;
		case StatType::MP:
			currentStat = player->_StatCom->GetMaxMp();
			break;
		case StatType::STR:
			currentStat = player->_StatCom->GetStr();
			break;
		case StatType::DEX:
			currentStat = player->_StatCom->GetDex();
			break;
		case StatType::INT:
			currentStat = player->_StatCom->GetInt();
			break;
		default:
			return; // 처리하지 않는 타입
		}

		// 증가/감소 처리
		if (amount > 0) // 증가
		{
			if (bonusPoints > 0)
			{
				switch (type)
				{
				case StatType::HP:
					player->_StatCom->SetMaxHp(currentStat + amount);
					break;
				case StatType::MP:
					player->_StatCom->SetMaxMp(currentStat + amount);
					break;
				case StatType::STR:
					player->_StatCom->SetStr(currentStat + amount);
					break;
				case StatType::DEX:
					player->_StatCom->SetDex(currentStat + amount);
					break;
				case StatType::INT:
					player->_StatCom->SetInt(currentStat + amount);
					break;
				}

				player->_StatCom->SetBonusPoint(bonusPoints - 1);
			}
		}
		else if (amount < 0) // 감소
		{
			if (currentStat > baseStat) // 기준 스탯 이하로 감소하지 않도록
			{
				switch (type)
				{
				case StatType::HP:
					player->_StatCom->SetMaxHp(currentStat + amount);
					break;
				case StatType::MP:
					player->_StatCom->SetMaxMp(currentStat + amount);
					break;
				case StatType::STR:
					player->_StatCom->SetStr(currentStat + amount);
					break;
				case StatType::DEX:
					player->_StatCom->SetDex(currentStat + amount);
					break;
				case StatType::INT:
					player->_StatCom->SetInt(currentStat + amount);
					break;
				}

				player->_StatCom->SetBonusPoint(bonusPoints + 1);
			}
		}

		// 스탯 UI 업데이트
	}
		UpdateStatDisplay();
}

void UStatWidget::PlLevelUpdate(int32 Level)
{
	if (PlLevel)
	{
		PlLevel->SetText(FText::AsNumber(Level));
	}
}

void UStatWidget::HPUpdate(int32 HP)
{
	if (HPNum)
	{
		HPNum->SetText(FText::AsNumber(HP));
		
	}
}

void UStatWidget::MPUpdate(int32 MP)
{
	if (MPNum != nullptr)
	{
		MPNum->SetText(FText::AsNumber(MP));
	}
}

void UStatWidget::STRUpdate(int32 STR)
{
	if (STRNum != nullptr)
	{
		STRNum->SetText(FText::AsNumber(STR));
	}
}

void UStatWidget::DEXUpdate(int32 DEX)
{
	if (DEXNum != nullptr)
	{
		DEXNum->SetText(FText::AsNumber(DEX));
	}
}

void UStatWidget::INTUpdate(int32 INT)
{
	if (INTNum != nullptr)
	{
		INTNum->SetText(FText::AsNumber(INT));
	}
}

void UStatWidget::BonusPointUpdate(int32 BonusPoint)
{
	if (BonusNum != nullptr)
	{
		BonusNum->SetText(FText::AsNumber(BonusPoint));
	}
}


void UStatWidget::UpdateStatDisplay()
{
	AMyPlayer* player = Cast<AMyPlayer>(GetOwningPlayerPawn());

	if (player && player->_StatCom)
	{
		StatUpdate(StatType::HP, player->_StatCom->GetMaxHp());
		StatUpdate(StatType::MP, player->_StatCom->GetMaxMp());
		StatUpdate(StatType::STR, player->_StatCom->GetStr());
		StatUpdate(StatType::DEX, player->_StatCom->GetDex());
		StatUpdate(StatType::INT, player->_StatCom->GetInt());
		
		BonusPointUpdate(player->_StatCom->GetBonusPoint());
		PlLevelUpdate(player->_StatCom->GetLevel());

		// 스탯 Log
		UE_LOG(LogTemp, Warning, TEXT("HP: %d, MP: %d, STR: %d, DEX: %d, INT: %d, EXP: %d, BonusPoint: %d"),
			player->_StatCom->GetMaxHp(),
			player->_StatCom->GetMaxMp(),
			player->_StatCom->GetStr(),
			player->_StatCom->GetDex(),
			player->_StatCom->GetInt(),
			player->_StatCom->GetExp(),
			player->_StatCom->GetBonusPoint());

	}
}

void UStatWidget::HPDownClick()
{
	
	StatUpdate(StatType::HP, -100); // HP를 100 감소
}


void UStatWidget::HPUpClick()
{

	StatUpdate(StatType::HP, 100); // HP를 100 증가

}

void UStatWidget::MPDownClick()
{
	StatUpdate(StatType::MP, -10); // MP를 10 감소

}

void UStatWidget::MpUpClick()
{
	StatUpdate(StatType::MP, 10); // MP를 10 증가
}

void UStatWidget::STRDownClick()
{
	StatUpdate(StatType::STR, -1); // STR을 1 감소

}


void UStatWidget::STRUpClick()
{
	StatUpdate(StatType::STR, 1); // STR을 1 증가

}

void UStatWidget::DEXDownClick()
{
	StatUpdate(StatType::DEX, -1); // DEX를 1 감소

}

void UStatWidget::DEXUpClick()
{
	StatUpdate(StatType::DEX, 1); // DEX를 1 증가

}

void UStatWidget::INTDownClick()
{
	StatUpdate(StatType::INT, -1); // INT를 1 감소

}

void UStatWidget::INTUpClick()
{
	StatUpdate(StatType::INT, 1); // INT를 1 증가

}
