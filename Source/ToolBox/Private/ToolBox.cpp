#include "ToolBox.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "ToolMenus.h"
#include "Engine/Engine.h"
#include "LevelEditor.h"

#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/SelectRobot/SelectRobotActor.h"
#include "Tools/Shake/ShakeActorTest.h"
#include "Tools/SlowMotion/SlowMotionActorTest.h"
#define LOCTEXT_NAMESPACE "FToolBoxModule"


void FToolBoxModule::StartupModule()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	const TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());

	MenuExtender->AddMenuBarExtension(
		"Help",
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateRaw(this, &FToolBoxModule::AddMenu)
	);

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}


void FToolBoxModule::AddMenu(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		FText::FromString("ToolBox"),
		FText::FromString("ToolBox Tip"),
		FNewMenuDelegate::CreateRaw(this, &FToolBoxModule::FillMenu));
}

void FToolBoxModule::FillMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Camera Shake Test"),
		FText::FromString("Camera Shake Test ToolTip"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			if (UWorld* World = GEditor->PlayWorld)
			{
				if (World->SpawnActor<AShakeActorTest>(AShakeActorTest::StaticClass()))
				{
					UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Spawned"), true);
				}
			}
			else
			{
				UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Start Play"), false);
			}
		})));


	MenuBuilder.AddMenuEntry(
		FText::FromString("SlowMotion Test"),
		FText::FromString("SlowMotion Test ToolTip"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{

			if (UWorld* World = GEditor->PlayWorld)
			{
				if (World->SpawnActor<ASlowMotionActorTest>(ASlowMotionActorTest::StaticClass()))
				{
					UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Spawned"), true);
				}
			}
			else
			{
				UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Start Play"), false);
			}
			
		}))
	);


	MenuBuilder.AddMenuEntry(
		FText::FromString("Select Robot"),
		FText::FromString("Select Robot ToolTip"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{

			if (UWorld* World = GEditor->PlayWorld)
			{
				if (World->SpawnActor<ASelectRobotActor>(ASelectRobotActor::StaticClass()))
				{
					UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Spawned"), true);
				}
			}
			else
			{
				UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Start Play"), false);
			}
			
		}))
	);
}

void FToolBoxModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToolBoxModule, ToolBox)
