using System.IO;
using UnrealBuildTool;



public class UnrealDeveloperEx : ModuleRules
{
	public UnrealDeveloperEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
#if UE_4_21_OR_LATER
#if UE_4_24_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.V2;
#endif
		PrivatePCHHeaderFile = "Private/UnrealDeveloperExPrivatePCH.h";
#endif

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
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEngineEx",
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
