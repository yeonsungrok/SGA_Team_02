// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSunder.generated.h"

UCLASS()
class PROTOTYPE_API ABossSunder : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossSunder();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Activate();
    void Deactivate();

    bool IsActive() const;

private:
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Effects",meta = (AllowPrivateAccess = "true"))
    class UNiagaraComponent* LightningEffect;

     UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Effects",meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* CollisionBox;

	bool bIsActive = false;

};
