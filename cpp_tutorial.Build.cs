// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class cpp_tutorial : ModuleRules
{
	public cpp_tutorial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
