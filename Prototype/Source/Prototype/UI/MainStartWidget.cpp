// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainStartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Base/Managers/UIManager.h"
#include "Base/MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"



UMainStartWidget::UMainStartWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SoundTest
	(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/options_UITEST.options_UITEST_C'"));
	if (SoundTest.Succeeded())
	{
		_Sound = SoundTest.Class;
	}
}

void UMainStartWidget::NativeConstruct()
{

	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainStartWidget::StartButtonClick);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainStartWidget::QuitButtonClick);
	}

	if (SoundButton)
	{
		SoundButton->OnClicked.AddDynamic(this, &UMainStartWidget::SoundButtonClick);
	}

}

void UMainStartWidget::StartButtonClick()
{
	UIManager->OpenUI(UI_LIST::Load);
	
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
}

void UMainStartWidget::QuitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainStartWidget::SoundButtonClick()
{
	if (_Sound)
	{
		UUserWidget* SoundTestrr = CreateWidget<UUserWidget>(GetWorld(), _Sound);
		if (SoundTestrr)
		{
			SoundTestrr->AddToViewport();
		}
	}
}
