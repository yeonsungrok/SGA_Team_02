// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class ABaseItem;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateShopList(TArray<ABaseItem*> list);
};
