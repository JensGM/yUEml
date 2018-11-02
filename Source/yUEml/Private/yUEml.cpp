// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "yUEml.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FyUEmlModule"

void FyUEmlModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("yUEml")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("ThirdParty/bin/yaml.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("ThirdParty/bin/yaml.dylib"));
#endif // PLATFORM_WINDOWS

	YamlLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (!YamlLibraryHandle)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
}

void FyUEmlModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(YamlLibraryHandle);
	YamlLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FyUEmlModule, yUEml)
