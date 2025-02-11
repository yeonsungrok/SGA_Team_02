// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalGameModeBase.h"
#include "Stage2NormalGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API AStage2NormalGameModeBase : public ANormalGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

};
