using System.IO;
using UnrealBuildTool;



public class ComponentEx : ModuleRules
{
	public ComponentEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
#if UE_4_21_OR_LATER
		PrivatePCHHeaderFile = "Private/ComponentExPrivatePCH.h";
#endif

		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});


		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});


		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
