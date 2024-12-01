// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class UCanvasPanel;
class ABaseItem;
class UIndexedButton;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual bool Initialize() override;

public:
	void UpdateShopList(TArray<ABaseItem*> list);

private:
	/// <summary>
	/// Inventory Elements
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Texture, meta = (AllowPrivateAccess = "true"))
	FString _defaultText;

	UPROPERTY()
	ABaseItem* _targetItem = nullptr;
	UPROPERTY()
	int32 _targetIndex = -1;

	//UI Elememts
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ItemSlots;
	UPROPERTY(meta = (BindWidget))
	TArray<UIndexedButton*> Button_;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemTexture;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDesc;

	UPROPERTY(meta = (BindWidget))
	class UButton* DropBtn;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DropBtnText;
	UPROPERTY(meta = (BindWidget))
	class UButton* SellBtn;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SellBtnText;

	//////////////////////////////////
	///Shop Elements

	TArray<UCanvasPanel*> _SellPanels;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SellPanel_0;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SellPanel_1;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SellPanel_2;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SellPanel_3;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SellPanel_4;

};
