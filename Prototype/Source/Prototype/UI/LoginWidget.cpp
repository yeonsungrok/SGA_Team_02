// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Base/MyGameInstance.h"

void ULoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LoginButton)
    {
        LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoginButton is not bound!"));
    }
}

void ULoginWidget::OnLoginButtonClicked()
{
    FString ID = IDTextBox->GetText().ToString();
    FString PW = PasswordTextBox->GetText().ToString();

    if (ID.IsEmpty() || PW.IsEmpty())
    {
        MessageText->SetText(FText::FromString("ID와 비밀번호를 입력해주세요."));
        return;
    }

    GAMEINSTANCE->Login(ID, PW);
}