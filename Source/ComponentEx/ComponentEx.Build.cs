using System.IO;
using UnrealBuildTool;



public class ComponentEx : ModuleRules
{
	public ComponentEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		bEnforceIWYU = true;
		DefaultBuildSettings = BuildSettingsVersion.V2;


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

				"AIModule",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"CoreEx"
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
