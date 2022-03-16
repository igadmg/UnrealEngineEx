using System.IO;
using UnrealBuildTool;



public class CoreEx : ModuleRules
{
	public CoreEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		bEnforceIWYU = true;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		PrivatePCHHeaderFile = "Private/CoreExPrivatePCH.h";


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
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
