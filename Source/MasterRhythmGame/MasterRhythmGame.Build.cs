// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MasterRhythmGame : ModuleRules
{
	public MasterRhythmGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"MIDIDevice", 
			"Harmonix", 
			"AudioMixer", 
			"MetasoundFrontend", 
			"MetasoundGraphCore", 
			"MetasoundEditor", 
			"MetasoundEngine", 
			"MetasoundGenerator", 
			"MetasoundEngineTest", 
			"MetasoundGraphCore", 
			"MetasoundStandardNodes", 
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
