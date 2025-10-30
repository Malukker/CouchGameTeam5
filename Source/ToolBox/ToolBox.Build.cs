using UnrealBuildTool;

public class ToolBox : ModuleRules
{
	public ToolBox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"Blutility",
			"UMG",
			"AssetRegistry"
		});
	}
}