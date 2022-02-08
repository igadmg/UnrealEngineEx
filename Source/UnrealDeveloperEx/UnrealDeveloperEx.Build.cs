using System.IO;
using UnrealBuildTool;



public class UnrealDeveloperEx : ModuleRules
{
	public UnrealDeveloperEx(ReadOnlyTargetRules Target)
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
				"BlueprintGraph",
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"CoreEx"
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"KismetCompiler",
				"SlateCore",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEngineEx",
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
