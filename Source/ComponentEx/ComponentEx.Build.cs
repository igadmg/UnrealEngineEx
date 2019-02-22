using System.IO;
using UnrealBuildTool;



public class ComponentEx : ModuleRules
{
	public ComponentEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Private/ComponentExPrivatePCH.h";

		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});


		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
