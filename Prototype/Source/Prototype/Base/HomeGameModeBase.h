// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameModeBase.h"
#include "HomeGameModebase.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API AHomeGameModebase : public AMyGameModeBase
{
	GENERATED_BODY()

public:
	AHomeGameModebase();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APortal_Stage2_Normal> _portal2;
};
