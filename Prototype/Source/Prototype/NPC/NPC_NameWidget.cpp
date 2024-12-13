// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPC_NameWidget.h"
#include "Components/TextBlock.h"

void UNPC_NameWidget::SetNPC_Name(FString name)
{
	if (name.IsEmpty())
		NPC_name->SetText(FText::FromString(TEXT("No data")));
	else
		NPC_name->SetText(FText::FromString(name));
}
