// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossFireball.generated.h"

UCLASS()
class PROTOTYPE_API ABossFireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossFireball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	 void LaunchTowards(FVector TargetLocation);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UStaticMeshComponent* Mesh;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
               UPrimitiveComponent* OtherComponent, FVector NormalImpulse, 
               const FHitResult& Hit);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 50.0f;

};
