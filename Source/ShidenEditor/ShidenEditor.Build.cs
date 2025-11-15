// Copyright (c) 2025 HANON. All Rights Reserved.

using UnrealBuildTool;

public class ShidenEditor : ModuleRules
{
	public ShidenEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"ApplicationCore",
				"ShidenCore",
				"JsonUtilities",
				"Json"
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Blutility",
				"UMG",
				"UMGEditor",
				"InputCore",
				"UnrealEd",
				"ShidenCore",
				"ContentBrowserData",
				"ToolWidgets",
				"PropertyEditor",
				"AssetTools",
				"ToolMenus"
				// ... add private dependencies that you statically link with here ...	
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}