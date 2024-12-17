// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourComponent_Test.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Player/MyPlayer.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UParkourComponent_Test::UParkourComponent_Test()
{
    PrimaryComponentTick.bCanEverTick = true;

    bIsClimbing = false;
    ClimbSpeed = 10.0f;
}

void UParkourComponent_Test::BeginPlay()
{
    Super::BeginPlay();
    Character = Cast<AMyPlayer>(GetOwner());
}

void UParkourComponent_Test::StartClimbing(FVector WallNormal)
{
    if (Character)
    {
        bIsClimbing = true;
        CurrentWallNormal = WallNormal;

        Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
    }
}

void UParkourComponent_Test::ClimbMove(float AxisValueForward, float AxisValueRight)
{
    if (bIsClimbing && Character)
    {
        FVector MoveDirection = FVector::ZeroVector;

        if (FMath::Abs(AxisValueForward) > KINDA_SMALL_NUMBER || FMath::Abs(AxisValueRight) > KINDA_SMALL_NUMBER)
        {
            if (FMath::Abs(AxisValueForward) > KINDA_SMALL_NUMBER)
            {
                MoveDirection += FVector::UpVector * AxisValueForward;
            }

            if (FMath::Abs(AxisValueRight) > KINDA_SMALL_NUMBER)
            {
                FVector RightVector = FVector::CrossProduct(CurrentWallNormal, FVector::UpVector);
                MoveDirection += RightVector * AxisValueRight;
            }

            MoveDirection = MoveDirection.GetSafeNormal();
            Character->AddMovementInput(MoveDirection, ClimbSpeed * 0.05f);
        }
        else
        {
            Character->GetCharacterMovement()->Velocity = FVector::ZeroVector;
        }
    }
}


void UParkourComponent_Test::StopClimbing()
{
    if (bIsClimbing && Character)
    {
        bIsClimbing = false;
        Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}
