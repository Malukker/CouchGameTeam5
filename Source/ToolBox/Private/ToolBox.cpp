#include "ToolBox.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "ToolMenus.h"
#include "Engine/Engine.h"
#include "LevelEditor.h"
#include "Tools/ShakeCameraTest.h"
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
			if (UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
			{
				const FString Path = TEXT("/Game/Tools/EUW_ShakeCameraTest.EUW_ShakeCameraTest");
				if (UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *Path))
				{
					EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBP);
				}else
				{
					FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Widget not found! Check the path.")));
				}
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


