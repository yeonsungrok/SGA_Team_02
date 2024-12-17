// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/SoundEffect.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

ASoundEffect::ASoundEffect()
{
	PrimaryActorTick.bCanEverTick = false;

	_audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = _audioComponent;
	_audioComponent->bAutoActivate = false;


}

void ASoundEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoundEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoundEffect::Play(FVector location)
{

	if (_audioComponent && _soundCom)
	{
		SetActorLocation(location);
		_audioComponent->SetSound(_soundCom);
		_audioComponent->Play();
	}
}

bool ASoundEffect::IsPlaying() const
{
	return _audioComponent->IsPlaying();
}

void ASoundEffect::Stop()
{
	if (IsPlaying())
	{
		_audioComponent->Stop();
	}
}

float ASoundEffect::GetDuration() const
{
	float Duration = _soundCom ? _soundCom->GetDuration() : 0.1f;
	UE_LOG(LogTemp, Warning, TEXT("Sound Duration: %f"), Duration);
	return Duration;
}




