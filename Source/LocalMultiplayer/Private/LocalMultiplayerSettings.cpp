// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"
#include "EnhancedInput/Public/InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const {
	for (FEnhancedActionKeyMapping ActionKey : GetIMCFromType(MappingType)->GetMappings()) {
		if (ActionKey.Key == Key) return true;
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const {
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::InGameUp:
		return IMCInGameUp;
		break;
	case ELocalMultiplayerInputMappingType::InGameDown:
		return IMCInGameDown;
		break;
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
		break;
	default:
		return nullptr;
		break;
	}
}