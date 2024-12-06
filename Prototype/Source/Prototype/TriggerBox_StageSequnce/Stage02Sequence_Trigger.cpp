// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBox_StageSequnce/Stage02Sequence_Trigger.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../Player/MyPlayer.h"
#include "../Player/Dragon.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"


#include "../Base/Managers/UIManager.h"

#include "UI/PlayerBarWidget.h"
#include "UI/SkillWidget_test.h"
#include "UI/MiniMapWidget.h"
#include "UI/StatWidget.h"
#include "Component/StatComponent.h"

#include "Base/Stage2BossGameModeBase.h"
#include <Base/MyGameInstance.h>
#include "Player/Creature.h"
#include "Blueprint/UserWidget.h"

#include "GameFramework/PlayerController.h"
#include "Player/MyPlayerController.h"



// Sets default values
AStage02Sequence_Trigger::AStage02Sequence_Trigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    TriggerBox02 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox02;

    TriggerBox02->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f)); // 크기 설정
    TriggerBox02->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox02->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    TriggerBox02->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    TriggerBox02->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AStage02Sequence_Trigger::BeginPlay()
{
	Super::BeginPlay();
	
    TriggerBox02->OnComponentBeginOverlap.AddDynamic(this, &AStage02Sequence_Trigger::OnTriggerEnter);

    // 시퀀스 로드
    ULevelSequence* LoadedSequence02 = LevelSequenceAsset02.LoadSynchronous();
    if (LoadedSequence02)
    {
        UE_LOG(LogTemp, Log, TEXT("LevelSequence loaded successfully."));

        FMovieSceneSequencePlaybackSettings PlaybackSettings;
        PlaybackSettings.bPauseAtEnd = true;

        // 시퀀스 플레이어 및 액터 생성
        ALevelSequenceActor* OutActor = nullptr;
        ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            LoadedSequence02,
            PlaybackSettings,
            OutActor);

        if (Player && OutActor)
        {
            SequenceActor02 = OutActor;

            // 시퀀스 종료 이벤트 등록
            Player->OnFinished.AddDynamic(this, &AStage02Sequence_Trigger::OnSequenceFinished);
        }
    }

}

// Called every frame
void AStage02Sequence_Trigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStage02Sequence_Trigger::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
   /* UIManager->CloseUI(UI_LIST::Boss);*/

    ACreature* Creature = Cast<ACreature>(OtherActor);
    if (Creature)
    {
        // 트리거된 액터의 위젯 숨기기
        UUserWidget* Widget = Creature->GetWidget();
        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Hidden);
        }

        // 트리거된 액터를 기록
        TriggeredActor = OtherActor;

        // 스킬 위젯
        HideSkillWidget();

        // 미니맵 블루프린트 시도
        UE_LOG(LogTemp, Error, TEXT("Broadcasting OnHideMinimap Delegate"));
        OnHideMinimap_02.Broadcast();

        if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
        {
            PlaySequence();
        }
    }


}

void AStage02Sequence_Trigger::PlaySequence()
{
    if (SequenceActor02)
    {
        ULevelSequencePlayer* Player = SequenceActor02->GetSequencePlayer();
        if (Player)
        {
             Player->Play();
        }
    }
}

void AStage02Sequence_Trigger::OnSequenceFinished()
{
   // UIManager->OpenUI(UI_LIST::Boss);

    // 기사 체력바 위젯
    if (TriggeredActor && TriggeredActor->IsA(ACreature::StaticClass()))
    {
        ACreature* Creature = Cast<ACreature>(TriggeredActor);
        if (Creature && Creature->_Widget)
        {
            Creature->_Widget->SetVisibility(ESlateVisibility::Visible);
        }
    }

    // 스킬 위젯
    ShowSkillWidget();

    // 미니맵 블루프린트 시도
    UE_LOG(LogTemp, Error, TEXT("Broadcasting OnShowMinimap Delegate"));
    OnShowMinimap_02.Broadcast();

 /*   AStage2BossGameModeBase* GameMode = Cast<AStage2BossGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->BossStart();
    }*/


    Destroy();

}

void AStage02Sequence_Trigger::HideSkillWidget()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);
        if (MyPlayerController && MyPlayerController->SkillWidgetInstance)
        {
            MyPlayerController->SkillWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void AStage02Sequence_Trigger::ShowSkillWidget()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);
        if (MyPlayerController && MyPlayerController->SkillWidgetInstance)
        {
            MyPlayerController->SkillWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

