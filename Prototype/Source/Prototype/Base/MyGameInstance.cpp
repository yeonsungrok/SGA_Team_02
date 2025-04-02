// Fill out your copyright notice in the Description page of Project Settings.
#include "Base/MyGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/StatComponent.h"
#include "Component/InventoryComponent.h"
#include "Player/MyPlayer.h"
#include "Item/BaseItem.h"
#include "Item/Equip/EquipItem.h"
#include "Item/Consumes/ConsumeItem.h"
#include "../UI/InventoryWidget.h"
#include "../Item/Equip/EquipItem.h"
#include "../Component/InventoryComponent.h"
#include "Base/Managers/UIManager.h"
#include "Base/Managers/SoundManager.h"


UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> StatData(TEXT("/Script/Engine.DataTable'/Game/Data/StatDataTable.StatDataTable'"));

	if (StatData.Succeeded())
	{
		_statTable = StatData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EpicStatData(TEXT("/Script/Engine.DataTable'/Game/Data/EpicMonsterDataTable.EpicMonsterDataTable'"));

	if (EpicStatData.Succeeded())
	{
		_EpicstatTable = EpicStatData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterStatData(TEXT("/Script/Engine.DataTable'/Game/Data/NormalMonsterDataTable.NormalMonsterDataTable'"));

	if (MonsterStatData.Succeeded())
	{
		_MonsterstatTable = MonsterStatData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> BossStatData(TEXT("/Script/Engine.DataTable'/Game/Data/BossMonsterDataTable.BossMonsterDataTable'"));

	if (BossStatData.Succeeded())
	{
		_BossstatTable = BossStatData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ConsumItemData(TEXT("/Script/Engine.DataTable'/Game/Data/Item/ConsumeItem.ConsumeItem'"));

	if (ConsumItemData.Succeeded())
	{
		_ConsItemTable = ConsumItemData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EquipItemData(TEXT("/Script/Engine.DataTable'/Game/Data/Item/Equipitem.Equipitem'"));

	if (EquipItemData.Succeeded())
	{
		_EquipItemTable = EquipItemData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DragonStat(TEXT("/Script/Engine.DataTable'/Game/Data/DragonStatDataTable.DragonStatDataTable'"));

	if (DragonStat.Succeeded())
	{
		_DragonStatTable = DragonStat.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> shopList1(TEXT("/Script/Engine.DataTable'/Game/Data/Postion_ShopKeeper.Postion_ShopKeeper'"));
	if (shopList1.Succeeded())
	{
		_ShopLists.Add(shopList1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> shopList2(TEXT("/Script/Engine.DataTable'/Game/Data/BasicEquip_ShopKeeper.BasicEquip_ShopKeeper'"));
	if (shopList2.Succeeded())
	{
		_ShopLists.Add(shopList2.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> shopList3(TEXT("/Script/Engine.DataTable'/Game/Data/ShinyEquip_ShopKeeper.ShinyEquip_ShopKeeper'"));
	if (shopList3.Succeeded())
	{
		_ShopLists.Add(shopList3.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> shopList4(TEXT("/Script/Engine.DataTable'/Game/Data/ShadowEquip_ShopKeeper.ShadowEquip_ShopKeeper'"));
	if (shopList4.Succeeded())
	{
		_ShopLists.Add(shopList4.Object);
	}
	SavedSkeletalMeshes.SetNum(6);
}

void UMyGameInstance::SavePlayerStats(UStatComponent* StatComponent)
{
	if (!StatComponent || CurrentUsername.IsEmpty()) return;

	TSharedPtr<FJsonObject> StatsData = MakeShareable(new FJsonObject());
	StatsData->SetStringField(TEXT("username"), CurrentUsername);

	TSharedPtr<FJsonObject> Stats = MakeShareable(new FJsonObject());
	Stats->SetNumberField(TEXT("level"), StatComponent->GetLevel());
	Stats->SetNumberField(TEXT("maxHp"), StatComponent->GetMaxHp());
	Stats->SetNumberField(TEXT("curHp"), StatComponent->GetCurHp());
	Stats->SetNumberField(TEXT("maxMp"), StatComponent->GetMaxMp());
	Stats->SetNumberField(TEXT("curMp"), StatComponent->GetCurMp());
	Stats->SetNumberField(TEXT("str_"), StatComponent->GetStr());
	Stats->SetNumberField(TEXT("dex"), StatComponent->GetDex());
	Stats->SetNumberField(TEXT("int_"), StatComponent->GetInt());
	Stats->SetNumberField(TEXT("exp"), StatComponent->GetExp());
	Stats->SetNumberField(TEXT("nextExp"), StatComponent->GetNextExp());
	Stats->SetNumberField(TEXT("bonusPoint"), StatComponent->GetBonusPoint());
	Stats->SetNumberField(TEXT("attackRadius"), StatComponent->GetAttackRadius());
	Stats->SetNumberField(TEXT("attackRange"), StatComponent->GetAttackRange());

	StatsData->SetObjectField(TEXT("stats"), Stats);

	FString Output;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
	FJsonSerializer::Serialize(StatsData.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("http://127.0.0.1:5000/save_stats");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(Output);
	Request->ProcessRequest();
}


void UMyGameInstance::LoadPlayerStats(UStatComponent* StatComponent)
{
    if (!StatComponent || CurrentUsername.IsEmpty()) return;

    FString Url = FString::Printf(TEXT("http://127.0.0.1:5000/load_stats?username=%s"), *CurrentUsername);

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");

    Request->OnProcessRequestComplete().BindLambda([StatComponent](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("LoadPlayerStats - Request failed"));
            return;
        }

        TSharedPtr<FJsonObject> Json;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Res->GetContentAsString());

        if (FJsonSerializer::Deserialize(Reader, Json) && Json.IsValid())
        {
            // 로그 출력 (선택)
            for (auto& Pair : Json->Values)
            {
                FString Key = Pair.Key;
                FString Value;

                if (Pair.Value->Type == EJson::Number)
                    Value = FString::SanitizeFloat(Pair.Value->AsNumber());
                else if (Pair.Value->Type == EJson::String)
                    Value = Pair.Value->AsString();

                UE_LOG(LogTemp, Warning, TEXT("[Stats] %s: %s"), *Key, *Value);
            }

            // 값 적용
            StatComponent->SetLevel(Json->GetIntegerField(TEXT("level")));
            StatComponent->SetMaxHp(Json->GetIntegerField(TEXT("maxHp")));
            StatComponent->SetHp(Json->GetIntegerField(TEXT("curHp")));
            StatComponent->SetMaxMp(Json->GetIntegerField(TEXT("maxMp")));
            StatComponent->SetMp(Json->GetIntegerField(TEXT("curMp")));
            StatComponent->SetStr(Json->GetIntegerField(TEXT("str_")));
            StatComponent->SetDex(Json->GetIntegerField(TEXT("dex")));
            StatComponent->SetInt(Json->GetIntegerField(TEXT("int_")));
            StatComponent->SetExp(Json->GetIntegerField(TEXT("exp")));
            StatComponent->SetNextExp(Json->GetIntegerField(TEXT("nextExp")));
            StatComponent->SetBonusPoint(Json->GetIntegerField(TEXT("bonusPoint")));
            StatComponent->SetAttackRadius(Json->GetNumberField(TEXT("attackRadius")));
            StatComponent->SetAttackRange(Json->GetNumberField(TEXT("attackRange")));
            StatComponent->UpdateUI();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("LoadPlayerStats - JSON 파싱 실패 또는 응답이 유효하지 않음"));
        }
    });

    Request->ProcessRequest();
}


void UMyGameInstance::SaveInventory(UInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent || CurrentUsername.IsEmpty()) return;

	TArray<TSharedPtr<FJsonValue>> InventoryArray;
	for (ABaseItem* Item : InventoryComponent->GetItemSlots())
	{
		if (!Item) continue;

		TSharedPtr<FJsonObject> ItemJson = MakeShareable(new FJsonObject());
		ItemJson->SetNumberField(TEXT("code"), Item->GetCode());
		ItemJson->SetStringField(TEXT("name"), Item->GetName());
		ItemJson->SetStringField(TEXT("type"), FString::FromInt((int)Item->GetType()));
		ItemJson->SetStringField(TEXT("modTarget"), StaticEnum<StatType>()->GetNameStringByValue((int64)Item->GetModStat()));
		ItemJson->SetStringField(TEXT("description"), Item->GetDesc());
		ItemJson->SetNumberField(TEXT("price"), Item->GetPrice());
		ItemJson->SetNumberField(TEXT("value"), Item->GetValue());
		ItemJson->SetNumberField(TEXT("equip"), Item->GetEquip());
		InventoryArray.Add(MakeShareable(new FJsonValueObject(ItemJson)));
	}

	TSharedPtr<FJsonObject> EquipJson = MakeShareable(new FJsonObject());
	for (const auto& Pair : InventoryComponent->GetEquipSlots())
	{
		if (!Pair.Value) continue;
		TSharedPtr<FJsonObject> ItemData = MakeShareable(new FJsonObject());
		ItemData->SetNumberField(TEXT("code"), Pair.Value->GetCode());
		ItemData->SetStringField(TEXT("name"), Pair.Value->GetName());
		ItemData->SetStringField(TEXT("type"), FString::FromInt((int)Pair.Value->GetType()));
		ItemData->SetStringField(TEXT("modTarget"), StaticEnum<StatType>()->GetNameStringByValue((int64)Pair.Value->GetModStat()));
		ItemData->SetStringField(TEXT("description"), Pair.Value->GetDesc());
		ItemData->SetNumberField(TEXT("price"), Pair.Value->GetPrice());
		ItemData->SetNumberField(TEXT("value"), Pair.Value->GetValue());
		ItemData->SetNumberField(TEXT("equip"), (int)Pair.Value->GetEquipType());
		EquipJson->SetObjectField(Pair.Key, ItemData);
	}

	TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject());
	Root->SetStringField(TEXT("username"), CurrentUsername);
	Root->SetArrayField(TEXT("inventory"), InventoryArray);
	Root->SetObjectField(TEXT("equip"), EquipJson);
	Root->SetNumberField(TEXT("money"), InventoryComponent->GetMoney());

	FString Output;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("http://127.0.0.1:5000/save_inventory");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(Output);
	Request->ProcessRequest();
}

void UMyGameInstance::LoadInventory(UInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent || CurrentUsername.IsEmpty()) return;

	FString Url = FString::Printf(TEXT("http://127.0.0.1:5000/load_inventory?username=%s"), *CurrentUsername);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");

	Request->OnProcessRequestComplete().BindLambda([this, InventoryComponent](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (!bSuccess || !Res.IsValid()) return;

		TSharedPtr<FJsonObject> Json;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Res->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, Json))
		{
			InventoryComponent->InitSlot();

			TSet<int32> EquippedItemCodes;

			// 1. 장착된 아이템 먼저 처리
			const TSharedPtr<FJsonObject>* EquipMap;
			if (Json->TryGetObjectField(TEXT("equip"), EquipMap))
			{
				for (const auto& Pair : (*EquipMap)->Values)
				{
					TSharedPtr<FJsonObject> ItemObj = Pair.Value->AsObject();
					if (!ItemObj.IsValid()) continue;

					int32 Code = ItemObj->GetIntegerField(TEXT("code"));
					int32 EquipType = ItemObj->GetIntegerField(TEXT("equip"));

					AEquipItem* EquipItem = GetWorld()->SpawnActor<AEquipItem>();
					if (EquipItem)
					{
						EquipItem->SetItemWithCode(Code);
						EquipItem->SetEquipType(EquipType);
						InventoryComponent->AddItemToEquip(Pair.Key, EquipItem);
						EquippedItemCodes.Add(Code);
					}
				}
			}

			// 2. 인벤토리 아이템 처리
			const TArray<TSharedPtr<FJsonValue>> Items = Json->GetArrayField(TEXT("inventory"));
			for (const auto& Val : Items)
			{
				TSharedPtr<FJsonObject> ItemObj = Val->AsObject();
				if (!ItemObj.IsValid()) continue;

				int32 Code = ItemObj->GetIntegerField(TEXT("code"));
				if (EquippedItemCodes.Contains(Code)) continue; // 이미 장착한 아이템은 패스

				int Type = FCString::Atoi(*ItemObj->GetStringField(TEXT("type")));
				ABaseItem* NewItem = nullptr;

				if (Type == (int)ItemType::Consume)
					NewItem = GetWorld()->SpawnActor<AConsumeItem>();
				else if (Type == (int)ItemType::Equipment)
				{
					AEquipItem* Equip = GetWorld()->SpawnActor<AEquipItem>();
					Equip->SetEquipType(ItemObj->GetIntegerField(TEXT("equip")));
					NewItem = Equip;
				}

				if (NewItem)
				{
					NewItem->SetItemWithCode(Code);
					InventoryComponent->AddItemToSlot(NewItem);
				}
			}

			// 3. 돈 처리
			if (Json->HasTypedField<EJson::Number>("money"))
			{
				InventoryComponent->AddMoney(Json->GetIntegerField(TEXT("money")));
			}
		}
	});
	Request->ProcessRequest();
}



void UMyGameInstance::SavePlayerSkeletal(AMyPlayer* Player)
{
    if (!Player || CurrentUsername.IsEmpty()) return;

    TArray<TSharedPtr<FJsonValue>> Meshes;

    // Helper lambda for safe mesh name extraction
    auto GetMeshName = [](USkeletalMeshComponent* MeshComp) -> FString
    {
        if (!MeshComp) return TEXT("None");

        USkinnedAsset* Skinned = MeshComp->GetSkinnedAsset();
        if (!Skinned) return TEXT("None");

        return Skinned->GetName();
    };

    // Get 5 skeletal meshes with null safety
    Meshes.Add(MakeShareable(new FJsonValueString(GetMeshName(Player->GetMesh()))));
    Meshes.Add(MakeShareable(new FJsonValueString(GetMeshName(Player->GetLowerBodyMesh()))));
    Meshes.Add(MakeShareable(new FJsonValueString(GetMeshName(Player->GetShoulderBodyMesh()))));
    Meshes.Add(MakeShareable(new FJsonValueString(GetMeshName(Player->GetSwordBodyMesh()))));
    Meshes.Add(MakeShareable(new FJsonValueString(GetMeshName(Player->GetShieldBodyMesh()))));

    // JSON 구성
    TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject());
    Json->SetStringField(TEXT("username"), CurrentUsername);
    Json->SetArrayField(TEXT("skeletal"), Meshes);

    FString Output;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
    FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

    // HTTP 요청
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("http://127.0.0.1:5000/save_skeletal");
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(Output);
    Request->ProcessRequest();
}

void UMyGameInstance::LoadPlayerSkeletal(AMyPlayer* Player)
{
	if (!Player || CurrentUsername.IsEmpty()) return;

	FString Url = FString::Printf(TEXT("http://127.0.0.1:5000/load_skeletal?username=%s"), *CurrentUsername);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");

	Request->OnProcessRequestComplete().BindLambda([Player](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (!bSuccess || !Res.IsValid()) return;

		TSharedPtr<FJsonObject> Json;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Res->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, Json))
		{
			const TArray<TSharedPtr<FJsonValue>>* MeshArray;
			if (Json->TryGetArrayField(TEXT("skeletal"), MeshArray))
			{
				if (MeshArray->Num() >= 5)
				{
					auto TrySetMesh = [](USkeletalMeshComponent* Comp, const FString& MeshName)
					{
						if (!Comp || MeshName.Equals(TEXT("None"), ESearchCase::IgnoreCase)) return;

						FString Path = FString::Printf(TEXT("SkeletalMesh'/Game/Mesh/%s.%s'"), *MeshName, *MeshName);
						USkeletalMesh* Mesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *Path));

						UE_LOG(LogTemp, Warning, TEXT("[Skeletal] Loading mesh: %s"), *Path);

						if (Mesh)
						{
							Comp->SetSkeletalMesh(Mesh);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("[Skeletal] Failed to load: %s"), *Path);
						}
					};

					TrySetMesh(Player->GetMesh(), (*MeshArray)[0]->AsString());
					TrySetMesh(Player->GetLowerBodyMesh(), (*MeshArray)[1]->AsString());
					TrySetMesh(Player->GetShoulderBodyMesh(), (*MeshArray)[2]->AsString());
					TrySetMesh(Player->GetSwordBodyMesh(), (*MeshArray)[3]->AsString());
					TrySetMesh(Player->GetShieldBodyMesh(), (*MeshArray)[4]->AsString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[Skeletal] Skeletal field not found in JSON"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[Skeletal] Failed to parse skeletal JSON"));
		}
	});

	Request->ProcessRequest();
}



void UMyGameInstance::Login(FString Username, FString Password)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("http://127.0.0.1:5000/login"); // Flask 서버 주소
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");

    // JSON 데이터 만들기
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    JsonObject->SetStringField("username", Username);
    JsonObject->SetStringField("password", Password);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(OutputString);
    Request->OnProcessRequestComplete().BindUObject(this, &UMyGameInstance::OnLoginResponseReceived);
    Request->ProcessRequest();

}

void UMyGameInstance::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("로그인 요청 실패"));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("서버 응답: %s"), *ResponseString);

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        bool bSuccess = JsonObject->GetBoolField("success");
        FString Message = JsonObject->GetStringField("message");

        if (JsonObject->HasField("is_new"))
        {
            bool bIsNew = JsonObject->GetBoolField("is_new");
            _firstIn = bIsNew; 
        }

		if (bSuccess)
		{
			CurrentUsername = JsonObject->GetStringField("username");
			bool bFirstIn = JsonObject->GetBoolField("first_in");
			SetFirst(bFirstIn);  
		
			UE_LOG(LogTemp, Log, TEXT("로그인 성공: %s (first_in: %s)"), *Message, bFirstIn ? TEXT("true") : TEXT("false"));
			OnLoginSuccess.Broadcast();
		}
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("로그인 실패: %s"), *Message);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패"));
    }
}


TArray<ABaseItem *> UMyGameInstance::GetInvenItemList()
{
	return UIManager->GetInventoryUI()->GetInvenContents();
}

void UMyGameInstance::Init()
{
	Super::Init();

	InitializeManagers();
}

void UMyGameInstance::InitializeManagers()
{
	FActorSpawnParameters params;
	params.Name = TEXT("UIManager");
	_UIManager = GetWorld()->SpawnActor<AUIManager>(FVector::ZeroVector, FRotator::ZeroRotator, params);

	FActorSpawnParameters soundParams;
	soundParams.Name = TEXT("SoundManager");
	_soundManager = GetWorld()->SpawnActor<ASoundManager>(FVector::ZeroVector, FRotator::ZeroRotator, soundParams);

	FActorSpawnParameters effectParams;
	effectParams.Name = TEXT("EffectManager");
	_effectManager = GetWorld()->SpawnActor<AEffectManager>(FVector::ZeroVector, FRotator::ZeroRotator, effectParams);
}

FMyStatData *UMyGameInstance::GetStatDataByLevel(int level)
{
	auto statData = _statTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return statData;
}

FMyStatData *UMyGameInstance::GetMonsterDataByLevel(int level)
{
	auto MonsterData = _MonsterstatTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return MonsterData;
}

FMyStatData *UMyGameInstance::GetEpicDataByLevel(int level)
{
	auto EpicStatData = _EpicstatTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return EpicStatData;
}

FMyStatData *UMyGameInstance::GetBossDataByLevel(int level)
{
	auto BossStatData = _BossstatTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return BossStatData;
}

FMyStatData *UMyGameInstance::GetDragonDataByLevel(int level)
{
	auto DragonStatData = _DragonStatTable->FindRow<FMyStatData>(*FString::FromInt(level), TEXT(""));
	return DragonStatData;
}

FItemData *UMyGameInstance::GetConsumeItemData(int code)
{
	auto ConsumeData = _ConsItemTable->FindRow<FItemData>(*FString::FromInt(code), TEXT(""));
	return ConsumeData;
}

FItemData *UMyGameInstance::GetEquipItemData(int code)
{
	auto EquipData = _EquipItemTable->FindRow<FItemData>(*FString::FromInt(code), TEXT(""));
	return EquipData;
}

TArray<FSellings *> UMyGameInstance::GetSellingData(int32 shop)
{
	TArray<FSellings *> sellingList;
	FSellings *sellingData = nullptr;

	for (int i = 0; i < SHOP_LIST_MAX; i++)
	{
		sellingData = _ShopLists[shop]->FindRow<FSellings>(*FString::FromInt(i), TEXT(""));
		sellingList.Add(sellingData);
	}

	return sellingList;
}

ABaseItem *UMyGameInstance::SellDataToItemData(FSellings *data)
{
	if (data->Type == ItemType::Equipment)
	{
		AEquipItem *equip = GetWorld()->SpawnActor<AEquipItem>(AEquipItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		equip->SetItemWithCode(data->Code);
		return Cast<ABaseItem>(equip);
	}
	else
	{
		ABaseItem *consume = GetWorld()->SpawnActor<ABaseItem>(ABaseItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		consume->SetItemWithCode(data->Code);
		return consume;
	}
}
