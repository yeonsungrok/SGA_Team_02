// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseItem.h" 
#include "EquipItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Helmet,
    UpperArmor,
    LowerArmor,
    ShoulderArmor,
    Sword,
    Shield,
    END
};

UCLASS()
class PROTOTYPE_API AEquipItem : public ABaseItem 
{
    GENERATED_BODY()
    
public:    
    AEquipItem();

    virtual void SetItemWithCode(int32 itemCode)override;

    ItemType GetArmorType(){return _Type;}
    EItemType GetEquipType(){return _equipItemType;}
    void SetEquipType(int num);

public:
    virtual void BeginPlay() override;
  
    virtual void Tick(float DeltaTime) override;

    virtual void EquipPlayer();
    void UnEquip();

    virtual void UseItem() override;
    virtual void DropItem(FVector location, FRotator rotation = FRotator::ZeroRotator) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Socket")
    FName AttachSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem")
    EItemType _equipItemType;

private:
    bool _isEquipped = false;

public:  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem")
    class USkeletalMesh* _equipItem;
};
