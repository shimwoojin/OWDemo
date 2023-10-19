// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OWDemo : ModuleRules
{
	public OWDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"UMG",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"Slate",
			"SlateCore",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"OWDemoSetting" 
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"OWDemo/Public/GameMake",
			"OWDemo/Public/GameMake/Structure",
			"OWDemo/Public/GameMake/Enum",
			"OWDemo/Public/GameMake/SaveGame",
			"OWDemo/Public/GameMake/DataAsset",
			"OWDemo/Public/GamePlay/Character",
			"OWDemo/Public/GamePlay/Character/Animation",
			"OWDemo/Public/GamePlay/Character/Controller",
			"OWDemo/Public/GamePlay/Character/AI",
			"OWDemo/Public/GamePlay/Interface",
			"OWDemo/Public/GamePlay/Interface/Management",
			"OWDemo/Public/GamePlay/Item",
			"OWDemo/Public/GamePlay/Item/Weapon",
			"OWDemo/Public/GamePlay/Component",
			"OWDemo/Public/GamePlay/Skill",
			"OWDemo/Public/GamePlay/Notify",
			"OWDemo/Public/GamePlay/Projectile",
			"OWDemo/Public/GamePlay/Projectile/Bullet",
			"OWDemo/Public/GamePlay/UI",
			"OWDemo/Public/GamePlay/CameraShake",
			"OWDemo/Public/GamePlay/Trigger"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
