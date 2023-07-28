// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemNakama : ModuleRules
{
	public OnlineSubsystemNakama(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDefinitions.Add("ONLINESUBSYSTEMNAKAMA_PACKAGE=1");
		
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"OnlineSubsystem",
				"NakamaCore",
				"NakamaUnreal",
			});
	}
}
