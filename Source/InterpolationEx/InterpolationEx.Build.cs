using System.IO;
using System.Diagnostics;
using UnrealBuildTool;



public class InterpolationEx : ModuleRules
{
	public InterpolationEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//PrivatePCHHeaderFile = "Private/InterpolationExPrivatePCH.h";


		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});

		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"InterpolationLibraryImport"
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
				"InputCore",
			});
	}
}

