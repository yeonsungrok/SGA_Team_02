// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/Managers/UIManager.h"

#include "UI/InventoryWidget.h"
#include "UI/Boss1Widget.h"
#include "UI/ShopWidget.h"
#include "Kismet/GameplayStatics.h"

#include "TriggerBox_StageSequnce/StageSequence_Trigger.h"

// Sets default values
AUIManager::AUIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> inventory(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Inventory_UI.Inventory_UI_C'"));
	if (inventory.Succeeded())
	{
		_inventoryUI = CreateWidget<UInventoryWidget>(GetWorld(), inventory.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> boss1widget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Boss_UI.Boss_UI_C'"));
	if (boss1widget.Succeeded())
	{
		_bossUI = CreateWidget<UBoss1Widget>(GetWorld(), boss1widget.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> shopUI(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Shop_UI.Shop_UI_C'"));
	if (shopUI.Succeeded())
	{
		_shopUI = CreateWidget<UShopWidget>(GetWorld(), shopUI.Class);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> defaultTexture(
		TEXT("/Script/Engine.Texture2D'/Game/CraftResourcesIcons/Textures/T_Default.T_Default'"));
	if (defaultTexture.Succeeded())
	{
		_defaultTexture = defaultTexture.Object;
	}

	_uiList.Add(_inventoryUI);
	_uiIsOpen.Add(false);
	_isPauseWhenOpen.Add(true);

	_uiList.Add(_bossUI);
	_uiIsOpen.Add(false);
	_isPauseWhenOpen.Add(false);

	_uiList.Add(_shopUI);
	_uiIsOpen.Add(false);
	_isPauseWhenOpen.Add(true);
}

// Called when the game starts or when spawned
void AUIManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUIManager::OpenUI(UI_LIST ui)
{
	int32 UIindex = (int32)ui;
	if (UIindex > _uiList.Num())
		return;

	if (_isPauseWhenOpen[UIindex])
		pauseGame.Broadcast();

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		bool bIsCursorVisible = PlayerController->bShowMouseCursor;
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
	}

	_uiList[UIindex]->SetVisibility(ESlateVisibility::Visible);
	int32 ZOrder = (ui == UI_LIST::Inventory) ? 10 : 0;
	_uiList[UIindex]->AddToViewport(ZOrder);

	_uiIsOpen[UIindex] = true;
}

void AUIManager::CloseUI(UI_LIST ui)
{
	int32 UIindex = (int32)ui;
	if (UIindex > _uiList.Num())
		return;

	if (_isPauseWhenOpen[UIindex])
		resumGame.Broadcast();

	_uiList[UIindex]->SetVisibility(ESlateVisibility::Hidden);
	_uiList[UIindex]->RemoveFromParent();
	_uiIsOpen[UIindex] = false;

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void AUIManager::CloseAll()
{
	resumGame.Broadcast();

	for (auto widget : _uiList)
	{
		widget->SetVisibility(ESlateVisibility::Hidden);
		widget->RemoveFromParent();
	}
	for (bool isopen : _uiIsOpen)
		isopen = false;
}

void AUIManager::ToggleUI(UI_LIST ui)
{
	int32 UIindex = (int32)ui;
	if (UIindex > _uiList.Num())
		return;

	if (_uiIsOpen[UIindex])
		CloseUI(ui);
	else
		OpenUI(ui);
}
