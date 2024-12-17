// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossSunder.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABossSunder::ABossSunder()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

    LightningEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningEffect"));

}

// Called when the game starts or when spawned
void ABossSunder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossSunder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossSunder::Activate()
{
	 SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    bIsActive = true;
}

void ABossSunder::Deactivate()
{
	  SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    bIsActive = false;
}

bool ABossSunder::IsActive() const
{
	 return bIsActive;
}

