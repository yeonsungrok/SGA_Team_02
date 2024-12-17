// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossObstacle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyedDelegate);

UCLASS()
class PROTOTYPE_API ABossObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossObstacle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDestroyedDelegate OnDestroyedEvent;

	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	virtual FString GetObstacleBreakEffect() const;
	virtual FString GetObstacleBreakSound() const;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ObstacleMesh;

};
