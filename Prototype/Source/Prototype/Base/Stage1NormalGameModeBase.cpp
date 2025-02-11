// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/Stage1NormalGameModeBase.h"


void AStage1NormalGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnMonster(FVector(-44.f,-6230.0f,-15.0f),10);
	SpawnMonster(FVector(-120.f,-13753.0f,-13.0f),5);
}








