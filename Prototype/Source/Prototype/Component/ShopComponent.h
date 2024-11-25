// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShopComponent.generated.h"

class ABaseItem;

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
	void SetSalesWithCode(int32 code);

	void Sale(int32 index);



private:
	TArray<ABaseItem*> _sallings;
};
