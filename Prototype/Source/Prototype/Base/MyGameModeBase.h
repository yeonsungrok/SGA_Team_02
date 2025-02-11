// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

protected:
    void InitializePlayer();
    void InitializeGameInstance();

	class AMyPlayer *Player;
	class UMyGameInstance *GameInstance;
	
};
