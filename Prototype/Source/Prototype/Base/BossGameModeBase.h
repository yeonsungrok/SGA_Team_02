// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/MyGameModeBase.h"
#include "BossGameModeBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROTOTYPE_API ABossGameModeBase : public AMyGameModeBase
{
	GENERATED_BODY()
	
public:
	ABossGameModeBase();
		
	virtual void BeginPlay() override;
 
	virtual void PostInitializeComponents() override;

	void BossClear();

	void BossStart();

	virtual void SpawnBoss() PURE_VIRTUAL(ABossGameModeBase::SpawnBoss, ;);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APortal_Home> _portal;

	UPROPERTY(AdvancedDisplay)
	FVector _portalLocation;

	UPROPERTY(AdvancedDisplay)
	FRotator _portalRotation;

	UPROPERTY(AdvancedDisplay)
	FVector _playerLocation;

	UPROPERTY(AdvancedDisplay)
	FRotator _playerRotation;

};
