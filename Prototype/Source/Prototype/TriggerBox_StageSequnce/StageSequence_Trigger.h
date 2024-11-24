// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "StageSequence_Trigger.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinimapVisibilityChanged);

class UUserWidget;
UCLASS()
class PROTOTYPE_API AStageSequence_Trigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageSequence_Trigger();

    // Minimap의 표시 상태 변경 이벤트
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMinimapVisibilityChanged OnHideMinimap;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMinimapVisibilityChanged OnShowMinimap;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // 트리거 박스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox;

    // 레벨 시퀀스 에셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
    TSoftObjectPtr<ULevelSequence> LevelSequenceAsset;

    // 시퀀스 액터
    UPROPERTY()
    ALevelSequenceActor* SequenceActor;

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
