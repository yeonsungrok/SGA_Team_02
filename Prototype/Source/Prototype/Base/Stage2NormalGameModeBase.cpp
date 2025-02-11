// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/Stage2NormalGameModeBase.h"


void AStage2NormalGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMonster(FVector(-1350.f,856.0f,209.0f),10);
	SpawnMonster(FVector(680.f,-1331.0f,1156.0f),10);
	
}

