#include "ToolBox.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "ToolMenus.h"
#include "Engine/Engine.h"
#include "LevelEditor.h"
#include "Tools/ShakeCameraTest.h"
#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/Shake/ShakeActorTest.h"
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
			// if (UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
			// {
			// 	const FString Path = TEXT("/Game/Tools/EUW_ShakeCameraTest.EUW_ShakeCameraTest");
			// 	if (UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *Path))
			// 	{
			// 		EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBP);
			// 	}else
			// 	{
			// 		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Widget not found! Check the path.")));
			// 	}
			// }


			if (UWorld* World = GEditor->PlayWorld)
			{
				
				FVector Location(0, 0, 200);
				FRotator Rotation(0, 0, 0);

				if (AShakeActorTest* ActorTest = World->SpawnActor<AShakeActorTest>(AShakeActorTest::StaticClass(), Location, Rotation))
				{
					// ActorTest->ShakeTestWidget = LoadClass<UUserWidget>(
					// 	nullptr,
					// 	TEXT("/Script/Engine.Blueprint'/Game/Tools/BP_ShakeActorTest.BP_ShakeActorTest_C")
					// );
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
		}))
	);
}

void FToolBoxModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToolBoxModule, ToolBox)
