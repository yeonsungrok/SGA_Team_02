// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Item/BaseItem.h"
#include "ShopComponent.generated.h"

class AMyPlayer;

#define SHOP_LIST_MAX 5

USTRUCT()
struct FSellings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Code;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShopComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void SetSales();

	void Sale(int32 index);


	void SetCustomer(AMyPlayer* target);

private:
	TArray<ABaseItem*> _sallings;

	AMyPlayer* _customer;
};
