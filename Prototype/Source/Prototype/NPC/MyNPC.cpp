// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPC.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/MyPlayer.h"
#include "Component/ShopComponent.h"

// Sets default values
AMyNPC::AMyNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	_trigger->SetupAttachment(RootComponent);
	_trigger->SetCollisionProfileName(TEXT("NPC"));
	_trigger->SetSphereRadius(100.0f);

	_skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	_skeletalMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> sm(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Scanned3DPeoplePack/RP_Character/rp_sophia_rigged_003_Mobile_ue4/rp_sophia_rigged_003_Mobile_ue4.rp_sophia_rigged_003_Mobile_ue4'")
	);
	if (sm.Succeeded())
	{
		_skeletalMesh->SetSkeletalMesh(sm.Object);
	}

	_shopComp = CreateDefaultSubobject<UShopComponent>(TEXT("Shop Component"));
}

// Called when the game starts or when spawned
void AMyNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyNPC::OnOverlapBegin);
	_trigger->OnComponentEndOverlap.AddDynamic(this, &AMyNPC::OnOverlapEnd);
}

void AMyNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	_target = Cast<AMyPlayer>(OtherActor);
	if (_target == nullptr)
		return;

	_isOverlapped = true;
	_shopComp->SetCustomer(_target);
	//Optional : Look Player
}

void AMyNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	_target = nullptr;
	_isOverlapped = false;
	_shopComp->SetCustomer(_target);
}

// Called every frame
void AMyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

