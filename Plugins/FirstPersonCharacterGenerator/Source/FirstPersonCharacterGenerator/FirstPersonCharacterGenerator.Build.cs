// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

using UnrealBuildTool;

public class FirstPersonCharacterGenerator : ModuleRules
{
	public FirstPersonCharacterGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "CoreUObject", "Engine"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd",
				"ToolMenus",
				"Slate",
				"SlateCore",
				"WorkspaceMenuStructure",
				"Projects",
				"UMG",
				"GameProjectGeneration",
				"DesktopPlatform"
			}
			);
	}
}
