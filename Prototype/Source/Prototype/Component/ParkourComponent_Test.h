// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent_Test.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UParkourComponent_Test : public UActorComponent
{
	GENERATED_BODY()
public:
	UParkourComponent_Test();

	bool bIsClimbing;

protected:
	virtual void BeginPlay() override;

public:	
	void StartClimbing(FVector WallNormal);

	void ClimbMove(float AxisValueForward, float AxisValueRight);

	void StopClimbing();

	UPROPERTY()
	class AMyPlayer* Character;

	FVector CurrentWallNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Climbing")
	float ClimbSpeed;
};