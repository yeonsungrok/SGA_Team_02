#include "Item/Equip/EquipItem.h"
#include "Base/MyGameInstance.h"
#include "Item/BaseItem.h"
#include "../../Player/MyPlayer.h"
#include "Component/StatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AEquipItem::AEquipItem()
{
    PrimaryActorTick.bCanEverTick = true;

    _Type = ItemType::Equipment;

    _trigger->SetSphereRadius(100.0f);
    _trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AEquipItem::SetItemWithCode(int32 itemCode)
{
    auto gameinstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
    if (gameinstance != nullptr)
    {
        FItemData* data = gameinstance->GetEquipItemData(itemCode);
        if (data == nullptr || data->_Name == TEXT(""))
        {
            UE_LOG(LogTemp, Error, TEXT("Data Load Faild!"));
            return;
        }

        _Code = data->_Code;
        _Texture = data->_Texture;
        _Mesh = data->_Mesh;
        _equipItem = data->_Skeletal;
        _Value = data->_Value;
        _Price = data->_Price;
        _Name = data->_Name;
        _Type = data->_Type;
        _ModStatType = data->_ModTarget;
        _Description = data->_Description;

        _meshComponent->SetStaticMesh(_Mesh);
    }
}

// Called when the game starts or when spawned
void AEquipItem::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEquipItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEquipItem::EquipPlayer()
{
    if (_player == nullptr)
        return;

    if (_equipItem)
    {
        switch (_equipItemType)
        {
        case EItemType::UpperArmor:
            _player->GetMesh()->SetSkeletalMesh(_equipItem);
            break;
        case EItemType::LowerArmor:
            _player->_lowerBodyMesh->SetSkeletalMesh(_equipItem);
            break;
        case EItemType::ShoulderArmor:
            _player->_shoulderBodyMesh->SetSkeletalMesh(_equipItem);
            break;
        case EItemType::Sword:
            _player->_swordBodyMesh->SetSkeletalMesh(_equipItem);
            break;
        case EItemType::Shield:
            _player->_shieldBodyMesh->SetSkeletalMesh(_equipItem);
            break;
        default:
            break;
        }
    }
    _isEquipped = true;
}

void AEquipItem::UnEquip()
{
    if (_player == nullptr)
        return;

    //TODO : if put DEFAULT, recover to default skeletal mesh
    switch (_equipItemType)
    {
    case EItemType::UpperArmor:
        _player->EquipBaseBody();
        break;
    case EItemType::LowerArmor:
        _player->EquipBaseLower();
        break;
    case EItemType::ShoulderArmor:
        _player->EquipBaseShoulder();
        break;
    case EItemType::Sword:
        _player->_swordBodyMesh->SetSkeletalMesh(nullptr);
        break;
    case EItemType::Shield:
        _player->_shieldBodyMesh->SetSkeletalMesh(nullptr);
        break;
    default:
        break;
    }

    _player->GetStatComponent()->ModStat(_ModStatType, -_Value);
    _isEquipped = false;
}

void AEquipItem::UseItem()
{
    if (_player)
    {
        EquipPlayer();
        _player->ItemEquipped.Broadcast(this);

        _player->GetStatComponent()->ModStat(_ModStatType, _Value);
    }
}

void AEquipItem::DropItem(FVector location, FRotator rotation)
{
    Super::DropItem(location, rotation);

    if(_isEquipped)
        UnEquip();
}

