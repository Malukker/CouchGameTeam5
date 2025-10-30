// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#ifdef WITH_EDITOR
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#endif
#include "ImportationTool.generated.h"

struct FAssetData;

/**
 * 
 */

UCLASS()
class ROBOTBATTLE_API UImportationTool : public UEditorUtilityWidget
{
	GENERATED_BODY()

#pragma region Setup
	
public:
	UFUNCTION(BlueprintCallable)
	void SetSelectedAssets();

	UFUNCTION(BlueprintCallable)
	void CustomInitialize();
private:
	TArray<FAssetData> SelectedAssets;

#pragma endregion
	
#pragma region Batch Renaming

public:
	UFUNCTION(BlueprintCallable)
	void RenameSelectedAssets();

private:
	void AddPrefix(FAssetData InAsset);

#pragma endregion

#pragma region File Sorting

public:
	UFUNCTION(BlueprintCallable)
	void SortByOptions();

	UFUNCTION(BlueprintCallable)
	void SetFolderName(FString InName) { FolderName = InName; }

private:
	FString FolderName;
	
	void MoveAssetToFolder(FAssetData InAsset);

	TMap<UClass*, FString> ClassToPath;

#pragma endregion
};
