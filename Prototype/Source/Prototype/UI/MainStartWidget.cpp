// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainStartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"



UMainStartWidget::UMainStartWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> KeyUI(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Widget_Info.Widget_Info_C'")
	);

	if (KeyUI.Succeeded())
	{
		_Keyimage = KeyUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SoundTest
	(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/Sound_UI.Sound_UI_C'"));
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

	if (_Keyimage) 
	{
		UUserWidget* ControlInfoWidget = CreateWidget<UUserWidget>(GetWorld(), _Keyimage);
		if (ControlInfoWidget)
		{
			ControlInfoWidget->AddToViewport();
		}
	}
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
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
