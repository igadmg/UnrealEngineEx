using System.IO;
using UnrealBuildTool;



public class UnrealDeveloperEx : ModuleRules
{
	public UnrealDeveloperEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Private/UnrealDeveloperExPrivatePCH.h";

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
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"KismetCompiler",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
