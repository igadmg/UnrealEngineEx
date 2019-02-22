using System.IO;
using UnrealBuildTool;



public class UnrealEngineEx : ModuleRules
{
	public UnrealEngineEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Private/UnrealEngineExPrivatePCH.h";

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
				"CoordinateSystemEx",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"CoreUObject",
				"Engine",
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
