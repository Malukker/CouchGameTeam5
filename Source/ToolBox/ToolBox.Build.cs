using UnrealBuildTool;

public class ToolBox : ModuleRules
{
	public ToolBox(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "RobotBattle" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Blutility",
			"UMG",
			"AssetRegistry",
			"RobotBattle"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"EditorStyle",
			"UnrealEd",
			"ToolMenus",
			"LevelEditor",
			"UMGEditor",
			"EnhancedInput"
			
		});

		
		
	}
}