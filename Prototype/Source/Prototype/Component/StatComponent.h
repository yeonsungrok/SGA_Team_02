// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(DeathDelegate)

DECLARE_MULTICAST_DELEGATE_OneParam(PlHP, float);
DECLARE_MULTICAST_DELEGATE_OneParam(PlMP, float);
DECLARE_MULTICAST_DELEGATE_OneParam(PIEXP, float);
DECLARE_MULTICAST_DELEGATE_OneParam(PILevel, int32);

UENUM()
enum class StatType
{
	HP,
	MP,
	STR,
	DEX,
	INT,
	CurHP,
	CurMP
};

USTRUCT()
struct FMyStatData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 STR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DEX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 INT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BonusPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EXP;

	//FMyStatData()
	//	: level(1)
	//	, MaxHP(100)
	//	, MaxMP(50)
	//	, STR(10)
	//	, DEX(10)
	//	, INT(10)
	//	,BonusPoint(3)
	//	,EXP(100) {}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/*void SetStat();*/
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Reset();

	int32 GetCurHp() { return _curHp; }
	int32 GetMaxHp() { return _maxHp; }
	int32 GetLevel() { return _level; }
	int32 GetExp() { return _curExp; }
	int32 GetCurMp() { return _curMp; }
	int32 GetMaxMp() { return _maxMp; }
	//test
	int32 GetStr()  { return _str; }
	int32 GetDex()  { return _dex; }
	int32 GetInt()  { return _int; }
	int32 GetBonusPoint() { return _bonusPoint; }

	void SetLevel(int32 newLevel);
	void SetMaxHp(int32 newMaxHp);
	void SetMaxMp(int32 newMaxMp);
	void SetBonusPoint(int32 newBp);
	void SetStr(int32 newstr);
	void SetDex(int32 newdex);
	void SetInt(int32 newint);
	void SetExp(int32 newexp);
	
	void AddStat(StatType type, int32 amount = 1);

	void SetStatBoost(int32 rate);
	void SetStun(bool stun){_stunned = stun;}
	float HpRatio() { return _curHp / (float)_maxHp; }

	void SetHp(int32 hp);
	void SetMp(int32 mp);
	int AddCurHp(int32 amount);
	int AddCurMp(int32 amount);

	void AddAttackDamage(float amount);
	void AddExp(int32 amount);

	void ModStat(StatType stat, int32 amount);

	void SetLevelInit(int level);
	// cheol 에픽몬스터 
	void SetEpicLevelInit(int level);
	void SetBossLevelInit(int level);

	bool IsDead() { return _curHp <= 0; }

	DeathDelegate _deathDelegate;

	float EXpRatio() { return _curExp / (float)_nextExp; }
	PlHP _PlHPDelegate;
	PlMP _PlMPDelegate;
	PIEXP _PlEXPDelegate;
	PILevel _PILevelDelegate;
	
	//int attckDamage(int32 Attpower, int32 str,int32 dex,int32 _int);

	int32 GetBaseStat(StatType statType) const; // 기준 스탯 값 가져오기
	void DecreaseStat(StatType stat, int32 amount); // 스탯 감소 (기준 값 이하로는 감소하지 않음)


	virtual FString GetLevelUpName() const;
	virtual FString GetLevelUpSound() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _level;

	TArray<TMap<FString, int32>> _stats;
	/// _stats ::
	/// [HP][now] [HP][level_og] [HP][statUI] [HP][armor]
	/// [MP][now] [MP][level_og] [MP][statUI] [MP][armor]
	/// [INT][now] [INT][level_og] [INT][statUI] [INT][armor]
	/// ...	
	/// level_og => update initialize, when level up
	/// statUI	 => update when statUI update
	/// armor	 => update when exchange armor
	/// now		 => level_og + statUI + armor

	///////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _curHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _maxHp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _ogHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _modHp;

	////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _curMp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _maxMp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _ogMp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _modMp;

	/////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _str;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _ogStr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _modStr;
	
	//////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _dex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _ogDex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _modDex;
	
	///////////////////////////////////
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _int;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _ogInt;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _modInt;
	
	//////////////////////////////
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _attackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _bonusPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _curExp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 _nextExp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
    bool _stunned;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
    float _stunDuration;

	float _stunTimer;


};
