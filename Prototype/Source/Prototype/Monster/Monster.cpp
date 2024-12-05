// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Monster.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "../Player/MyPlayer.h"
#include "../Player/MyPlayerController.h"
#include "Item/BaseItem.h"
#include "Item/Consumes/ConsumeItem.h"
#include "Item/Equip/EquipItem.h"
#include "Base/MyGameInstance.h"
#include "Component/StatComponent.h"

AMonster::AMonster()
{
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

    _capsuleComponent = GetCapsuleComponent();
    _capsuleComponent->OnComponentHit.AddDynamic(this, &AMonster::OnHit);
}

void AMonster::BeginPlay()
{
    Super::BeginPlay();
}

void AMonster::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    _StatCom->SetLevelInit(1);
}

void AMonster::Disable()
{
    Super::Disable();
}

void AMonster::AttackHit()
{
    TArray<FHitResult> hitResults;
    FCollisionQueryParams params(NAME_None, false, this);

    float attackRange = 500.0f;
    float attackRadius = 50.0f;

    bool bResult = GetWorld()->SweepMultiByChannel(
        hitResults,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * attackRange,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(attackRadius),
        params);

    FVector vec = GetActorForwardVector() * attackRange;
    FVector center = GetActorLocation() + vec * 0.5f;

    FColor drawColor = FColor::Green;

    if (bResult)
    {
        drawColor = FColor::Red;

        for (auto &hitResult : hitResults)
        {
            AActor *hitActor = hitResult.GetActor();
            if (hitActor && hitActor->IsValidLowLevel())
            {
                if (!hitActor->IsA<AMonster>())
                {
                    FDamageEvent DamageEvent;
                    hitActor->TakeDamage(_StatCom->GetStr(), DamageEvent, GetController(), this);
                    _hitPoint = hitResult.ImpactPoint;

                    SoundManager->PlaySound(*GetHitSoundName(), _hitPoint);
                    EffectManager->Play(*GetPlayerAttackHitEffect(), _hitPoint);
                    break;
                }
            }
        }
    }
    else
    {
        FVector missLocation = GetActorLocation();
        SoundManager->PlaySound(*GetSwingSoundName(), missLocation);
    }

    DrawDebugSphere(GetWorld(), center, attackRadius, 32, drawColor, false, 0.3f);
}

void AMonster::DropReword()
{
}

float AMonster::TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
        return 0.0f;

    AMyPlayer *Player = Cast<AMyPlayer>(PlayerController->GetPawn());

    float damaged = -_StatCom->AddCurHp(-Damage);

    if (this->_StatCom->IsDead() && Player != nullptr)
    {
        SetActorEnableCollision(false);
        auto controller = GetController();
        if (controller)
            GetController()->UnPossess();
        MonsterEvent.Broadcast();

        Player->_inventoryComponent->GettingMoney(FMath::FRand() * 100);
        Player->_StatCom->AddExp(_StatCom->GetNextExp());
        const float RewardChance = 0.3f;

        if (FMath::FRand() <= RewardChance)
        {
            Reward(Player);
        }
    }
    return 0.0f;
}

void AMonster::Reward(AMyPlayer *player)
{
    ABaseItem *NewItem = nullptr;

    // UE_LOG(LogTemp, Warning, TEXT("Reward"));
    // int RandomValue = FMath::RandRange(2, 3);
    // NewItem = GetWorld()->SpawnActor<AConsumeItem>(AConsumeItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    // NewItem->SetItemWithCode(RandomValue);

    // if (NewItem)
    // {
    //     player->_inventoryComponent->AddItemToSlot(NewItem);
    // }
}

void AMonster::LaunchFromPlayer(FVector LaunchDirection)
{
    LaunchCharacter((LaunchDirection * _launchLength) + _upVector, true, true);
}

void AMonster::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    AMyPlayer *Player = Cast<AMyPlayer>(OtherActor);
    if (Player && Player->IsDashing())
    {
        Player->OnMonsterHit(this, Hit);
    }
}
