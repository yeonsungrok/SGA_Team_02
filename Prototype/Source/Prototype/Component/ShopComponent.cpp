// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShopComponent.h"

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

void UShopComponent::SetSalesWithCode(int32 code)
{
}

void UShopComponent::Sale(int32 index)
{
}

