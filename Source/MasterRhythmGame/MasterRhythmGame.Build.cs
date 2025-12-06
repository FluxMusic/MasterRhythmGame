// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MasterRhythmGame : ModuleRules
{
	public MasterRhythmGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MIDIDevice", "Harmonix", "AudioMixer" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
