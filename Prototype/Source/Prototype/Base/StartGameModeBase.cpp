// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"
#include "Base/MyGameInstance.h"
#include "Player/MyPlayer.h"
#include "UI/MainStartWidget.h"
#include "Blueprint/UserWidget.h" 

#include "Kismet/GameplayStatics.h"

AStartGameModeBase::AStartGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UMainStartWidget> StartWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/MainStart_UI.MainStart_UI_C'"));
	if (StartWidget.Succeeded())
	{
		StartLevelWidgetClass = StartWidget.Class;
	}

}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (StartLevelWidgetClass)
	{

		StartLevelWidget = CreateWidget<UMainStartWidget>(GetWorld(), StartLevelWidgetClass);

		if (StartLevelWidget)
		{
			StartLevelWidget->AddToViewport();
		}
		
	}

	AMyPlayer* player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			if (GameInstance->GetFirst())
			{
				UE_LOG(LogTemp, Warning, TEXT("First GamemOde"));
				GAMEINSTANCE->InitializeManagers();

				UStatComponent* StatComponent = player->FindComponentByClass<UStatComponent>();
				UInventoryComponent* InvenComponent = player->FindComponentByClass<UInventoryComponent>();
				if (StatComponent)
				{
					player->_StatCom->SetLevelInit(1);
				}
				if (InvenComponent)
				{
					player->_inventoryComponent->InitSlot();
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("no first GamemOde"));
				GAMEINSTANCE->InitializeManagers();

				UStatComponent* StatComponent = player->FindComponentByClass<UStatComponent>();
				UInventoryComponent* InvenComponent = player->FindComponentByClass<UInventoryComponent>();
				if (StatComponent)
				{
					GameInstance->LoadPlayerStats(StatComponent);
				}
				if (InvenComponent)
				{
					GameInstance->LoadInventory(InvenComponent);
				}
				GameInstance->LoadPlayerSkeletal(player);


			}
		}
	}
}

void AStartGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
