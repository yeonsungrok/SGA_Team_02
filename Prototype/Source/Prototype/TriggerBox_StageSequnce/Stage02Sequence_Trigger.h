// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

#include "Blueprint/UserWidget.h"
#include "Stage02Sequence_Trigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinimap02VisibilityChanged);

class UUserWidget;
UCLASS()
class PROTOTYPE_API AStage02Sequence_Trigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStage02Sequence_Trigger();

	// Minimap의 표시 상태 변경 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMinimap02VisibilityChanged OnHideMinimap_02;

	UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMinimap02VisibilityChanged OnShowMinimap_02;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox02;

    // 레벨 시퀀스 에셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
    TSoftObjectPtr<ULevelSequence> LevelSequenceAsset02;

    // 시퀀스 액터
    UPROPERTY()
    ALevelSequenceActor* SequenceActor02;

    // 트리거 진입 시 실행될 함수
    UFUNCTION()
    void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 시퀀스 재생 함수
    UFUNCTION()
    void PlaySequence();

    // 시퀀스 종료 처리 함수
    UFUNCTION()
    void OnSequenceFinished();

    AActor* TriggeredActor;

private:
    void HideSkillWidget();
    void ShowSkillWidget();


};
