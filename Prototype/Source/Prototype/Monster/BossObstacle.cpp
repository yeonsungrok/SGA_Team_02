// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossObstacle.h"
#include "BossMonster.h"
#include "Components/BoxComponent.h"
#include "../Base/Managers/EffectManager.h"
#include "../Base//Managers/SoundManager.h"
#include "EngineUtils.h"  
#include "Components/StaticMeshComponent.h"
#include "Base/MyGameInstance.h"

// Sets default values
ABossObstacle::ABossObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName("BlockAll");
	CollisionBox->OnComponentHit.AddDynamic(this,&ABossObstacle::OnHit);
	RootComponent = CollisionBox;

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
	ObstacleMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABossObstacle::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossObstacle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ABossMonster* boss = Cast<ABossMonster>(OtherActor);
	if(boss != nullptr &&(boss->GetIsJumping() || boss->GetIsDashing()))
	{
		boss->DashEnd();
		boss->DestroyObstacle();
		boss->_StatCom->SetStun(true);

		EffectManager->Play(*GetObstacleBreakEffect(), ObstacleMesh->GetComponentLocation());
		SoundManager->PlaySound(*GetObstacleBreakSound(), ObstacleMesh->GetComponentLocation());

		OnDestroyedEvent.Broadcast();

		Destroy();
	}
	
}

FString ABossObstacle::GetObstacleBreakEffect() const
{
	return "NS_LightningSlash";
}

FString ABossObstacle::GetObstacleBreakSound() const
{
	return "ObstacleBroken_Cue";
}

