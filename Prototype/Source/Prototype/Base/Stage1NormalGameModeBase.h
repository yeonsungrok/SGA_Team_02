// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalGameModeBase.h"
#include "Stage1NormalGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API AStage1NormalGameModeBase : public ANormalGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
