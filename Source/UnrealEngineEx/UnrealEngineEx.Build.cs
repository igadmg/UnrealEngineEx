using System.IO;
using UnrealBuildTool;



public class UnrealEngineEx : ModuleRules
{
	public UnrealEngineEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		PrivatePCHHeaderFile = "Private/UnrealEngineExPrivatePCH.h";


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
				"EnhancedInput",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"CoreEx",
				"ComponentEx",
				"CoordinateSystemEx",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"EngineSettings",
				"InputCore",
				"Json",
				"JsonUtilities",
				"HTTP",
				"SlateCore",
				"Slate",
				"UMG",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
					"UnrealEd",
				});
		}
	}
}
