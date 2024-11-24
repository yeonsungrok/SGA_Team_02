// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Dragon.h"
//-----추가-----
#include "Base/MyGameInstance.h"
#include "Player/MyPlayer.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

#include "../Animation/PlayerAnimInstance.h"
#include "../Animation/DragonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "../Animation/BaseAnimInstance.h"


#include "Base/Managers/UIManager.h"
#include "UI/StatWidget.h"
#include "UI/PlayerBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"


ADragon::ADragon()
{
	PrimaryActorTick.bCanEverTick = true;
	_knightInstance = nullptr;

    // 드래곤의 비행을 위해 중력 제거
    
    GetCapsuleComponent()->InitCapsuleSize(230.0f, 230.0f);
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    
    _springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    _camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    
    _springArm->SetupAttachment(RootComponent);
    _camera->SetupAttachment(_springArm);

    _springArm->TargetArmLength = 1200.0f;
    _springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));

    _springArm->bUsePawnControlRotation = false;
    _camera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = false;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> DragonMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
    if (DragonMeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(DragonMeshAsset.Object);
    }

    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -230.0f)); // 예: 메쉬를 살짝 아래로 배치
    GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));



    static ConstructorHelpers::FClassFinder<UStatWidget> StatClass(
        TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/PlayerStat_UI.PlayerStat_UI_C'"));
    if (StatClass.Succeeded())
    {
        _statWidget = CreateWidget<UStatWidget>(GetWorld(), StatClass.Class);
    }

    if (WidgetClass)
    {
        _Widget = CreateWidget<UPlayerBarWidget>(GetWorld(), WidgetClass);
        if (_Widget)
        {
            _Widget->AddToViewport();
        }
    }

}

void ADragon::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<AMyPlayer> It(GetWorld()); It; ++It)
    {
        _knightInstance = *It;
        if (_knightInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("KnightInstance found and assigned!"));
            break;
        }
    }

    if (!_knightInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("KnightInstance not found in the world!"));
        return;
    }

    // Dragon 초기화
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    _isTransformed = false;

}




void ADragon::TransformToHuman()
{
     if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        // 상태 복원
        _knightInstance->SetActorLocation(GetActorLocation());  // 현재 Dragon의 위치로 복원
        _knightInstance->SetActorRotation(GetActorRotation());

        // MyPlayer 활성화
        _knightInstance->SetActorHiddenInGame(false);
        _knightInstance->SetActorEnableCollision(true);

        // Dragon 비활성화
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        // 컨트롤 전환
        PC->Possess(_knightInstance);

        // 상태 업데이트
        _isTransformed = false;
        _knightInstance->_isTransformed = false;

        UE_LOG(LogTemp, Warning, TEXT("Transformed back to MyPlayer!"));
    }

}

void ADragon::TransformToDragon()
{
    // MyPlayer에서 Dragon으로 복귀
    _knightInstance->TransformToDragon();
}

void ADragon::ToggleTransformation()
{
    if (_isTransformed) // 현재 변환된 상태이면 인간으로 복귀
    {
        TransformToHuman();
    }
    else // 그렇지 않으면 드래곤으로 변환
    {
        TransformToDragon();
    }
}

void ADragon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &ADragon::Move);
        EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &ADragon::Look);
        EnhancedInputComponent->BindAction(_jumpAction, ETriggerEvent::Started, this, &ADragon::JumpA);     
        EnhancedInputComponent->BindAction(_Change, ETriggerEvent::Started, this, &ADragon::ToggleTransformation);
    
    }
}

void ADragon::Move(const FInputActionValue& value)
{
    // fly할때 isFalling 통해서 생각해볼것..
    FVector2D MovementVector = value.Get<FVector2D>();
           
    if (Controller && MovementVector.Size() > 0.0f)
    {
        // 이동 방향 벡터 계산
        const FRotator ControlRotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, ControlRotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y); // 전진/후진
        AddMovementInput(RightDirection, MovementVector.X);   // 좌우 이동
    }
}

void ADragon::Look(const FInputActionValue& value)
{
    FVector2D LookAxisVector = value.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X); // 좌우 회전
    AddControllerPitchInput(LookAxisVector.Y); // 상하 회전

}

void ADragon::JumpA(const FInputActionValue& value)
{
    bool isPressed = value.Get<bool>();

    if (isPressed)
    {
        if (GetCharacterMovement()->IsFalling())
        {
            // 공중에서 추가 비행 (날갯짓)
            FVector ForwardInput = GetActorForwardVector() * GetInputAxisValue("MoveForward");
            FVector RightInput = GetActorRightVector() * GetInputAxisValue("MoveRight");
            FVector JumpImpulse = ForwardInput * 300.0f + RightInput * 300.0f + FVector(0.0f, 0.0f, 500.0f);

            LaunchCharacter(JumpImpulse, true, true);

    /*        if (_dragonAnimInstance)
            {
                _dragonAnimInstance->SetSpeed(GetVelocity().Size());
            }*/

            UE_LOG(LogTemp, Warning, TEXT("Dragon flapped its wings!"));
        }
        else
        {
            // 지상에서 점프
            ACharacter::Jump();

            // 공중 제어 및 중력 설정
            GetCharacterMovement()->GravityScale = 0.5f; // 느린 하강
            GetCharacterMovement()->AirControl = 0.8f;    // 공중 제어 강화
            GetCharacterMovement()->MaxFlySpeed = 800.0f; // 공중 속도 증가

            UE_LOG(LogTemp, Warning, TEXT("Dragon jumped!"));

   /*         if (_dragonAnimInstance)
            {
                _dragonAnimInstance->SetSpeed(GetVelocity().Size());
            }*/

        }



        // 애니메이션 상태 업데이트: 점프 시작
        _dragonAnimInstance->SetJumping(true);
    }
    else
    {
        if (!GetCharacterMovement()->IsFalling())
        {
            // 착지 시 중력 및 이동 속도 복원
            GetCharacterMovement()->GravityScale = 1.0f;
            GetCharacterMovement()->AirControl = 0.2f; // 기본 공중 제어 값
            GetCharacterMovement()->MaxFlySpeed = 600.0f; // 기본 속도 복원
            
            // 애니메이션 상태 업데이트: 착지
            _dragonAnimInstance->SetJumping(false);

       /*     if (_dragonAnimInstance)
            {
                _dragonAnimInstance->SetSpeed(GetVelocity().Size());
            }*/

            UE_LOG(LogTemp, Warning, TEXT("Dragon landed!"));
        }
    }



}

void ADragon::PostInitializeComponents()
{
    Super::PostInitializeComponents();

      // 애니메이션 인스턴스 클래스를 설정
    if (DragonAnimInstanceClass)
    {
        USkeletalMeshComponent* MeshComp = GetMesh();
        if (MeshComp)
        {
            MeshComp->SetAnimInstanceClass(DragonAnimInstanceClass);
        }
    }

    // 애니메이션 인스턴스 캐스팅
    _dragonAnimInstance = Cast<UDragonAnimInstance>(GetMesh()->GetAnimInstance());
    if (_dragonAnimInstance && _dragonAnimInstance->IsValidLowLevelFast())
    {
        // 애니메이션 몽타주 끝났을 때 처리할 델리게이트 연결
        _dragonAnimInstance->OnMontageEnded.AddDynamic(this, &ADragon::OnAttackEnded);

        // 드래곤 사망 델리게이트 연결
        _dragonAnimInstance->_deathDelegate_Dragon.AddUObject(this, &ADragon::Disable);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to initialize _dragonAnimInstance for ADragon!"));
    }

    //----------------------------------------------보스몹으로 일단세팅----------------------
    if (_StatCom)
    {
        _StatCom->SetBossLevelInit(1);
    }
    //------------------------------------------------------------------------------------------
    if (_Widget)
    {
        auto PlWidget = Cast<UPlayerBarWidget>(_Widget);
        if (PlWidget)
        {
            float CurrentHP = _StatCom->GetCurHp();
            float CurrentMP = _StatCom->GetCurMp();
            float CurrentEXP = _StatCom->GetExp();

            // 플레이어 스탯
            UE_LOG(LogTemp, Warning, TEXT("Current HP: %f, Current MP: %f, Current EXP: %f"), CurrentHP, CurrentMP, CurrentEXP);

            _StatCom->_PlHPDelegate.AddUObject(PlWidget, &UPlayerBarWidget::SetPlHPBar);
            _StatCom->_PlMPDelegate.AddUObject(PlWidget, &UPlayerBarWidget::SetPlMPBar);
            _StatCom->_PlEXPDelegate.AddUObject(PlWidget, &UPlayerBarWidget::SetPlExpBar);
        }
    }

}

void ADragon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    if (_StatCom->IsDead())
        return;

    if (_Widget)
    {
        auto PlWidget = Cast<UPlayerBarWidget>(_Widget);
        if (PlWidget)
        {
            int32 PlMaxHp = _StatCom->GetMaxHp();
            int32 PlMaxMp = _StatCom->GetMaxMp();
            int32 PlCurHp = _StatCom->GetCurHp();
            int32 PlCurMp = _StatCom->GetCurMp();

            float HPPercent = float(PlCurHp) / float(PlMaxHp);
            float MPPercent = float(PlCurMp) / float(PlMaxMp);

            float NewHPScaleX = float(PlMaxHp) / 1000.0f;
            float NewMPScaleX = float(PlMaxMp) / 50.0f;

            if (_StatCom->GetMaxHp() > _StatCom->GetCurHp())
            {
                PlWidget->Pl_HPBar->SetPercent(HPPercent);
                PlWidget->Pl_HPBar->SetRenderScale(FVector2D(NewHPScaleX, 3.0f));
            }

            if (_StatCom->GetMaxMp() > _StatCom->GetCurMp())
            {
                PlWidget->Pl_MPBar->SetPercent(MPPercent);
                PlWidget->Pl_MPBar->SetRenderScale(FVector2D(NewMPScaleX, 3.0f));
            }
        }
    }


    if (GetVelocity().Size() > 0.0f)
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw = FMath::Atan2(GetVelocity().Y, GetVelocity().X) * 180.0f / PI;  // 이동 방향에 맞게 회전
        SetActorRotation(NewRotation); // 회전 적용
    }

}
