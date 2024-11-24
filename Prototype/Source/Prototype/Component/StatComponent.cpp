// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatComponent.h"
#include "Base/MyGameInstance.h"
#include "Base/Managers/UIManager.h"
#include "UI/InventoryWidget.h"
#include "Player/MyPlayer.h"
#include "Player/MyPlayerController.h"
#include "TimerManager.h"

#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Base/Managers/SoundManager.h"
#include "../Base/Managers/EffectManager.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



}




// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


	

}



// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::Reset()
{
	_curHp = _maxHp;
}



void UStatComponent::SetLevelInit(int level)
{
	FMyStatData* Data = nullptr;

	if (GAMEINSTANCE)
	{
		Data = GAMEINSTANCE->GetStatDataByLevel(level);
		_level = level;
		_maxHp = Data->MaxHP;
		_maxMp = Data->MaxMP;
		_str = Data->STR;
		_dex = Data->DEX;
		_int = Data->INT;

		_nextExp = Data->EXP;
		_curExp = 0;
		SetHp(_maxHp);
		SetMp(_maxMp);
		_bonusPoint = Data->BonusPoint;
		_PILevelDelegate.Broadcast(_level);

		if (_level == 1)
		{
			_ogHp = _maxHp;
			_ogMp = _maxMp;
			_ogStr = _str;
			_ogDex = _dex;
			_ogInt = _int;
		}

		if (Cast<AMyPlayer>(GetOwner()))
		{
			UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::HP, _maxHp);
			UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::MP, _maxMp);
			UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::STR, _str);
			UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::DEX, _dex);
			UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::INT, _int);

			UIManager->GetInventoryUI()->UpdateStat();
		}
	}

}

void UStatComponent::SetEpicLevelInit(int level)
{
	FMyStatData* Data = nullptr;

	if (GAMEINSTANCE)
	{
		Data = GAMEINSTANCE->GetEpicDataByLevel(level);
		_level = level;
		_maxHp = Data->MaxHP;
		_ogHp = _maxHp;
		_maxMp = Data->MaxMP;
		_ogMp = _maxMp;
		_str = Data->STR;
		_ogStr = _str;
		_dex = Data->DEX;
		_ogDex = _dex;
		_int = Data->INT;
		_ogInt = _int;

		_nextExp = Data->EXP;
		_curExp = 0;
		SetHp(_maxHp);
		SetMp(_maxMp);
		_bonusPoint = Data->BonusPoint;
		_PILevelDelegate.Broadcast(_level);

	}
}

void UStatComponent::SetBossLevelInit(int level)
{
	FMyStatData* Data = nullptr;

	if (GAMEINSTANCE)
	{
		Data = GAMEINSTANCE->GetBossDataByLevel(level);
		_level = level;
		_maxHp = Data->MaxHP;
		_ogHp = _maxHp;
		_curHp = _maxHp;
		_maxMp = Data->MaxMP;
		_ogMp = _maxMp;
		_str = Data->STR;
		_ogStr = _str;
		_dex = Data->DEX;
		_ogDex = _dex;
		_int = Data->INT;
		_ogInt = _int;

		_nextExp = Data->EXP;
		SetHp(_maxHp);
		SetMp(_maxMp);
		_bonusPoint = Data->BonusPoint;
		_PILevelDelegate.Broadcast(_level);

	}
}

int32 UStatComponent::GetBaseStat(StatType statType) const
{
	switch (statType)
	{
	case StatType::HP:
		return _ogHp;
	case StatType::MP:
		return _ogMp;
	case StatType::STR:
		return _ogStr;
	case StatType::DEX:
		return _ogDex;
	case StatType::INT:
		return _ogInt;
	default:
		return 0;
	}
}

void UStatComponent::DecreaseStat(StatType stat, int32 amount)
{
	switch (stat)
	{
	case StatType::HP:
		if (_maxHp - amount >= _ogHp)
			_maxHp -= amount;
		break;
	case StatType::MP:
		if (_maxMp - amount >= _ogMp)
			_maxMp -= amount;
		break;
	case StatType::STR:
		if (_str - amount >= _ogStr)
			_str -= amount;
		break;
	case StatType::DEX:
		if (_dex - amount >= _ogDex)
			_dex -= amount;
		break;
	case StatType::INT:
		if (_int - amount >= _ogInt)
			_int -= amount;
		break;
	default:
		break;
	}
}

FString UStatComponent::GetLevelUpName() const
{
	return "P_Status_LevelUp";
}

FString UStatComponent::GetLevelUpSound() const
{
	return "LevelupSound_Cue";
}



void UStatComponent::SetLevel(int32 newLevel)
{
	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;

	_level = Data->level;
	_level = FMath::Clamp(newLevel, 0, 100);
}

void UStatComponent::SetMaxHp(int32 newMaxHp)
{

	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;

	_maxHp = Data->MaxHP;
	_maxHp = FMath::Clamp(newMaxHp, 0, 10000);

	UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::HP, _maxHp);
	UIManager->GetInventoryUI()->UpdateStat();
}

void UStatComponent::SetMaxMp(int32 newMaxMp)
{

	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;


	_maxMp = Data->MaxMP;
	_maxMp = FMath::Clamp(newMaxMp, 0, 10000);

	UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::MP, _maxMp);
	UIManager->GetInventoryUI()->UpdateStat();
}

void UStatComponent::SetBonusPoint(int32 newBp)
{

	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;

		_bonusPoint = Data->BonusPoint;
		_bonusPoint = FMath::Clamp(newBp, 0, 10000);
}

void UStatComponent::SetStr(int32 newstr)
{

	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;


	_str = Data->STR;
	_str = FMath::Clamp(newstr, 0, 100);

	UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::STR, _str);
	UIManager->GetInventoryUI()->UpdateStat();
}

void UStatComponent::SetDex(int32 newdex)
{

	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;


	_dex = Data->DEX;
	_dex = FMath::Clamp(newdex, 0, 100);

	UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::DEX, _dex);
	UIManager->GetInventoryUI()->UpdateStat();
}

void UStatComponent::SetInt(int32 newint)
{
	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;


	_int = Data->INT;
	_int = FMath::Clamp(newint, 0, 100);

	UIManager->GetInventoryUI()->UpdateOriginStat((int32)StatType::INT, _int);
	UIManager->GetInventoryUI()->UpdateStat();
}

void UStatComponent::SetExp(int32 newexp)
{
	auto myGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	FMyStatData* Data = nullptr;

	_curExp = FMath::Clamp(newexp, 0, 100);
}

void UStatComponent::AddStat(StatType type, int32 amount)
{
	switch (type)
	{
	case StatType::HP:
		//TODO : statUI += amount
		break;
	case StatType::MP:
		break;
	case StatType::STR:
		break;
	case StatType::DEX:
		break;
	case StatType::INT:
		break;
	case StatType::CurHP:
		break;
	case StatType::CurMP:
		break;
	default:
		break;
	}
}



void UStatComponent::SetStatBoost(int32 rate)
{

    float boostFactor = 1.0f + (rate / 100.0f);


    int32 originalStr = _str;
    int32 originalDex = _dex;
    int32 originalInt = _int;
    int32 originalMaxHp = _maxHp;
    int32 originalMaxMp = _maxMp;

    _str = FMath::Clamp(_str * boostFactor, 0, 100); 
    _dex = FMath::Clamp(_dex * boostFactor, 0, 100);
    _int = FMath::Clamp(_int * boostFactor, 0, 100);
    _maxHp = FMath::Clamp(_maxHp * boostFactor, 0, 10000); 
    _maxMp = FMath::Clamp(_maxMp * boostFactor, 0, 10000);

    SetHp(_maxHp); 
    SetMp(_maxMp); 

	UE_LOG(LogTemp, Warning, TEXT("Boost str: %d , _dex: %d, _int:%d, _maxHp :%d, _maxMp:%d"),_str,_dex,_int,_maxHp,_maxMp);

	FTimerHandle StatBoostTimerHandle;

    GetWorld()->GetTimerManager().SetTimer(StatBoostTimerHandle, [this, originalStr, originalDex, originalInt, originalMaxHp, originalMaxMp]()
    {
		UE_LOG(LogTemp, Warning, TEXT("End Boost"));
        _str = originalStr;
        _dex = originalDex;
        _int = originalInt;
        _maxHp = originalMaxHp;
        _maxMp = originalMaxMp;

        SetHp(_maxHp);
        SetMp(_maxMp); 
    }, 5.0f, false); 
}


void UStatComponent::SetHp(int32 hp)
{
	_curHp = hp;
	if (_curHp <= 0)
	{
		_deathDelegate.Broadcast();
		_deathDelegate.Clear();
		_curHp = 0;
	}
	if (_curHp > _maxHp)
	{
		_curHp = _maxHp;
	}

	float ratio = HpRatio();
	_PlHPDelegate.Broadcast(ratio);

}

void UStatComponent::SetMp(int32 mp)
{
	_curMp = mp;
	if (_curMp < 0)
	{
		_curMp = 0;
	}

	if (_curMp > _maxMp)
	{
		_curMp = _maxMp;
	}

	float ratio = _curMp / (float)_maxMp;

	_PlMPDelegate.Broadcast(ratio);
}

int UStatComponent::AddCurHp(int32 amount)
{
	int beforeHp = _curHp;

	int afterHp = beforeHp + amount;
	SetHp(afterHp);

	return afterHp - beforeHp;
}

int UStatComponent::AddCurMp(int32 amount)
{
	int beforeMp = _curMp;

	int afterMp = beforeMp + amount;
	SetMp(afterMp);

	return afterMp - beforeMp;
}

void UStatComponent::AddAttackDamage(float amount)
{
	_attackDamage += amount;
	UE_LOG(LogTemp, Warning, TEXT("Attack Damage: %d"), _attackDamage);
}

void UStatComponent::AddExp(int32 amount)
{
	if (_level == 3)
		return;
	_curExp += amount;

	while (_curExp >= _nextExp)
	{
		_curExp -= _nextExp;
		_level++;
		_nextExp = 100 + (_level * 50); 
		_PILevelDelegate.Broadcast(_level);

		_ogHp = _maxHp;
		_ogMp = _maxMp;
		_ogStr = _str;
		_ogDex = _dex;
		_ogInt = _int;

		_bonusPoint += 6;

		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GameInstance && GameInstance->GetEffectManager())
		{
			GameInstance->GetEffectManager()->Play(*GetLevelUpName(), GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
			
			GameInstance->GetSoundManager()->PlaySound(*GetLevelUpSound(), GetOwner()->GetActorLocation());
		}
	}
	float ratio = EXpRatio();
	_PlEXPDelegate.Broadcast(ratio);
}

void UStatComponent::ModStat(StatType stat, int32 amount)
{
	auto invenUI = GAMEINSTANCE->GetUIManager()->GetInventoryUI();

	switch (stat)
	{
	case StatType::HP:
		_modHp += amount;
		_maxHp = _ogHp + _modHp;
		invenUI->UpdateOriginStat((int32)(StatType::HP), _maxHp);
		break;
	case StatType::MP:
		_modMp += amount;
		_maxMp = _ogMp + _modMp;
		invenUI->UpdateOriginStat((int32)(StatType::MP), _maxMp);
		break;
	case StatType::STR:
		_modStr += amount;
		_str = _ogStr + _modStr;
		invenUI->UpdateOriginStat((int32)(StatType::STR), _str);
		break;
	case StatType::DEX:
		_modDex += amount;
		_dex = _ogDex + _modDex;
		invenUI->UpdateOriginStat((int32)(StatType::DEX), _dex);
		break;
	case StatType::INT:
		_modInt += amount;
		_int = _ogInt + _modInt;
		invenUI->UpdateOriginStat((int32)(StatType::INT), _int);
		break;
	case StatType::CurHP:
		AddCurHp(amount);
		break;
	case StatType::CurMP:
		AddCurMp(amount);
		break;
	default:
		break;
	}
}
