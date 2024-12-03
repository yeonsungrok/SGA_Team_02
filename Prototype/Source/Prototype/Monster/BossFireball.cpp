// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossFireball.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "../Player/MyPlayer.h"



// Sets default values
ABossFireball::ABossFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 0.0f;
    ProjectileMovement->MaxSpeed = 5000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ABossFireball::BeginPlay()
{
	Super::BeginPlay();

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossFireball::OnMyCharacterOverlap);	
}

// Called every frame
void ABossFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossFireball::LaunchTowards(FVector TargetLocation)
{
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    ProjectileMovement->Velocity = Direction * 2000.f;
}

void ABossFireball::OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto player = Cast<AMyPlayer>(OtherActor);
    if (player)
    {
        player->TakeDamage(_damageAmount, FDamageEvent(), nullptr, this);
        player->Silent();
        Destroy();
    }
}

