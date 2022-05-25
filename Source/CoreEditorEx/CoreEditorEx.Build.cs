using System.IO;
using UnrealBuildTool;


public class CoreEditorEx : ModuleRules
{
	public CoreEditorEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		PrivatePCHHeaderFile = "Private/CoreEditorExPrivatePCH.h";

		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});


		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});


		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"CoreEx"
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"CoreUObject",
				"Engine",
				"UnrealEd",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
