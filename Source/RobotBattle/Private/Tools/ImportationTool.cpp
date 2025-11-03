// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ImportationTool.h"

#ifdef WITH_EDITOR
#include "AssetSelection.h"
#endif
#include "Particles/ParticleSystem.h"
#include "PhysicsEngine/PhysicsAsset.h"
#ifdef WITH_EDITOR
#include "Subsystems/EditorAssetSubsystem.h"
#endif

#pragma region Setup

void UImportationTool::SetSelectedAssets()
{
	AssetSelectionUtils::GetSelectedAssets(SelectedAssets);
}

void UImportationTool::CustomInitialize()
{
	ClassToPath.Empty();
	ClassToPath.Add(UTexture::StaticClass(), "/Game/RobotBattleContent/Assets/Textures");
	ClassToPath.Add(UTexture2D::StaticClass(), "/Game/RobotBattleContent/Assets/Textures");
	ClassToPath.Add(UStaticMesh::StaticClass(), "/Game/RobotBattleContent/Assets/StaticMesh");
	ClassToPath.Add(UParticleSystem::StaticClass(), "/Game/RobotBattleContent/Assets/Particles");
	ClassToPath.Add(UAnimationAsset::StaticClass(), "/Game/RobotBattleContent/Assets/Animations");
	ClassToPath.Add(UMaterial::StaticClass(), "/Game/RobotBattleContent/Assets/Materials/Masters");
	ClassToPath.Add(UMaterialInstance::StaticClass(), "/Game/RobotBattleContent/Assets/Materials/Instances");
	ClassToPath.Add(USkeleton::StaticClass(), "/Game/RobotBattleContent/Assets/Skeletons");
	ClassToPath.Add(USkeletalMesh::StaticClass(), "/Game/RobotBattleContent/Assets/Skeletons");
	ClassToPath.Add(UPhysicsAsset::StaticClass(), "/Game/RobotBattleContent/Assets/Skeletons");
}

#pragma endregion

#pragma region Batch Rename

void UImportationTool::RenameSelectedAssets()
{
	for (auto AssetReference : SelectedAssets)
	{
		AddPrefix(AssetReference);
	}
}

void UImportationTool::AddPrefix(FAssetData InAsset)
{
	if (InAsset == nullptr) { return; }

	FString Name = InAsset.AssetName.ToString();

	FString NewName;
	UClass* AssetClass = InAsset.GetClass();
	if (AssetClass == nullptr) { return; }

	if (AssetClass->IsChildOf(UTexture::StaticClass()))
	{
		NewName = "T_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(UStaticMesh::StaticClass()))
	{
		NewName = "SM_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(USkeleton::StaticClass()))
	{
		NewName = "SK_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(USkeletalMesh::StaticClass()))
	{
		NewName = "SKM_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(UPhysicsAsset::StaticClass()))
	{
		NewName = "PA_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(UAnimationAsset::StaticClass()))
	{
		NewName = "A_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(UMaterial::StaticClass()))
	{
		NewName = "M_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(UMaterialInstance::StaticClass()))
	{
		NewName = "MI_";
		NewName.Append(Name);
	}
	if (AssetClass->IsChildOf(UParticleSystem::StaticClass()))
	{
		NewName = "P_";
		NewName.Append(Name);
	}

	FString PathToName = "";
	int Index = -1;
	int LastSlashIndex = -1;
	for (const auto Char : InAsset.GetObjectPathString().GetCharArray())
	{
		Index++;
		if (Char == '/')
		{
			LastSlashIndex = Index;
		}
		if (Char == '.')
		{
			break;
		}
	}
	for (int i = 0; i <= LastSlashIndex; i++)
	{
		PathToName += InAsset.GetObjectPathString().GetCharArray()[i];
	}
	PathToName += NewName;

	UEditorAssetSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	EditorSubsystem->RenameAsset
	(
		InAsset.GetObjectPathString(),
		PathToName
	);
}

#pragma endregion

#pragma region File Sorting

void UImportationTool::SortByOptions()
{
	for (auto AssetData : SelectedAssets)
	{
		MoveAssetToFolder(AssetData);
	}
}

void UImportationTool::MoveAssetToFolder(FAssetData InAsset)
{
	if (InAsset == nullptr) { return; }

	UEditorAssetSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	TArray<UClass*> Keys;
	ClassToPath.GetKeys(Keys);
	for (const auto Key : Keys)
	{
		UClass* Class = InAsset.GetClass();
		if (Key == Class)
		{
			FString NewPath = ClassToPath[Key];

			if (FolderName != "")
			{
				if (!EditorSubsystem->DoesDirectoryExist(NewPath + "/" + FolderName))
				{
					EditorSubsystem->MakeDirectory(NewPath + "/" + FolderName);
				}

				NewPath += + "/" + FolderName + "/" + InAsset.AssetName.ToString();

				EditorSubsystem->RenameAsset
							(
								InAsset.GetObjectPathString(),
								NewPath
							);

				break;
			}

			NewPath += "/" + InAsset.AssetName.ToString();

			EditorSubsystem->RenameAsset
			(
				InAsset.GetObjectPathString(),
				NewPath
			);

			break;
		}
	}
}

#pragma endregion
