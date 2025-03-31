// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UEditableText* IDTextBox;

    UPROPERTY(meta = (BindWidget))
    class UEditableText* PasswordTextBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* LoginButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MessageText;

private:
    UFUNCTION()
    void OnLoginButtonClicked();
	
};
