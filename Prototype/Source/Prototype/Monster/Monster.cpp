// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
#include "Components/CapsuleComponent.h"
#include "../Player/MyPlayer.h"
#include "../Player/MyPlayerController.h"
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

void AMonster::DropReword()
{
    
}

float AMonster::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser); 

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return 0.0f;

    AMyPlayer* Player = Cast<AMyPlayer>(PlayerController->GetPawn());

	float damaged = -_StatCom->AddCurHp(-Damage);
	UE_LOG(LogTemp, Warning, TEXT("Take Damaged: %f"),Damage);

	if (this->_StatCom->IsDead() && Player != nullptr)
	{
		SetActorEnableCollision(false);
		auto controller = GetController();
		if (controller)
			GetController()->UnPossess();
		MonsterEvent.Broadcast();
		Player->_StatCom->AddExp(GetExp());
	}
	return 0.0f;
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


