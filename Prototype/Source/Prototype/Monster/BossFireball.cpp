// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossFireball.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"



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
    ProjectileMovement->MaxSpeed = 1200.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossFireball::OnHit);

}

// Called when the game starts or when spawned
void ABossFireball::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossFireball::LaunchTowards(FVector TargetLocation)
{
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    ProjectileMovement->Velocity = Direction * 800.f;
}

void ABossFireball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
			   UPrimitiveComponent* OtherComponent, FVector NormalImpulse, 
			   const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, UDamageType::StaticClass());
    Destroy();
}

