// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class yUEml : ModuleRules
{
	public yUEml(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Projects"
			}
			);

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine"
            }
            );

        string RootDirectory = Path.Combine(ModuleDirectory, "../..");

        PublicLibraryPaths.Add(Path.Combine(RootDirectory, "ThirdParty/bin"));
        PublicIncludePaths.Add(Path.Combine(RootDirectory, "ThirdParty/include"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(RootDirectory, "ThirdParty/lib"));
            PublicAdditionalLibraries.Add("yaml.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("yaml.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add("yaml.dylib");
        }
    }
}
