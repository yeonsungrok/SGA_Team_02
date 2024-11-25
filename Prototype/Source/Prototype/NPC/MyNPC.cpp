// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPC.h"
#include "Components/SphereComponent.h"
#include "Player/MyPlayer.h"

// Sets default values
AMyNPC::AMyNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyNPC::PostInitializeComponents()
{
	_trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyNPC::OnOverlapBegin);
	_trigger->OnComponentEndOverlap.AddDynamic(this, &AMyNPC::OnOverlapEnd);
}

void AMyNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	_target = Cast<AMyPlayer>(OtherActor);
	if (_target == nullptr)
		return;

	//Look Player
}

void AMyNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	_target = nullptr;
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

