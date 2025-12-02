// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/AnimNotify/AnimPlayListSoundNotify.h"


#include "Sounds/SoundListData.h"

void UAnimPlayListSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Sound = GetSoundFromData();

	Super::Notify(MeshComp, Animation, EventReference);
}

USoundBase* UAnimPlayListSoundNotify::GetSoundFromData() const
{
	if (SoundList == nullptr)
		return nullptr;
	SoundList->LoadConfig();
	return SoundList->Sounds[FMath::RandRange(0, SoundList->Sounds.Num() - 1)];
}
