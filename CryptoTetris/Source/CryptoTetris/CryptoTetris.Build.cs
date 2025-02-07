// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CryptoTetris : ModuleRules
{
	public CryptoTetris(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore"
		});

		// Enable exception handling semantics
		bEnableExceptions = true;

		// Add the /EHsc flag
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PrivateDefinitions.Add("_HAS_EXCEPTIONS=1");
			PrivateDefinitions.Add("PLATFORM_EXCEPTIONS_DISABLED=0");
			PrivateDefinitions.Add("WIN32_LEAN_AND_MEAN");
			PrivateDefinitions.Add("USE_MALLOC_PROFILER=0");
			PrivateDefinitions.Add("GCOV=0");
			PrivateDefinitions.Add("USE_NEW_ALLOCATOR=1");
			PrivateDefinitions.Add("ALLOW_RESERVED_KEYWORDS=0");
		}

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
