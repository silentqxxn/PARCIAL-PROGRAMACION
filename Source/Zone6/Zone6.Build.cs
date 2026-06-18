// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Zone6 : ModuleRules
{
	public Zone6(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Zone6",
			"Zone6/Variant_Platforming",
			"Zone6/Variant_Platforming/Animation",
			"Zone6/Variant_Combat",
			"Zone6/Variant_Combat/AI",
			"Zone6/Variant_Combat/Animation",
			"Zone6/Variant_Combat/Gameplay",
			"Zone6/Variant_Combat/Interfaces",
			"Zone6/Variant_Combat/UI",
			"Zone6/Variant_SideScrolling",
			"Zone6/Variant_SideScrolling/AI",
			"Zone6/Variant_SideScrolling/Gameplay",
			"Zone6/Variant_SideScrolling/Interfaces",
			"Zone6/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
