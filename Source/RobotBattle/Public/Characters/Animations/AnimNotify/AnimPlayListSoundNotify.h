// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "AnimPlayListSoundNotify.generated.h"

class USoundListData;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API UAnimPlayListSoundNotify : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundListData* SoundList;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


private:
	USoundBase* GetSoundFromData() const;
};
