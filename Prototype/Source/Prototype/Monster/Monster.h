// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Creature.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Monster.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AMonster : public ACreature
{
	GENERATED_BODY()

public:
	AMonster();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void Disable() override;

	virtual void Attack_AI(){};
	

protected:
	virtual void DropReword();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* _capsuleComponent;


public:
	int32 GetExp() { return _exp; }
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void LaunchFromPlayer(FVector LaunchDirection);

	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//virtual void OnDeath() {};


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* _MonsterMinimapSprite;
protected:
	UPROPERTY(VisibleAnywhere)
	int32 _exp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Physics", meta = (AllowPrivateAccess = "true"))
    float _launchLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Physics", meta = (AllowPrivateAccess = "true"))
	FVector _upVector;


	//TODO::만들어지면
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	// class AAIController* _aiController;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ReWord, meta = (AllowPrivateAccess = "true"))
	// TArray<class ABaseItem*> _rewardItem;

	//UPROPERTY()
	//UClass* _rewardClass;
	
};
