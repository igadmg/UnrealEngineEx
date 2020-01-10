using System.IO;
using UnrealBuildTool;



public class CoordinateSystemEx : ModuleRules
{
	public CoordinateSystemEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
#if UE_4_21_OR_LATER
		PrivatePCHHeaderFile = "Private/CoordinateSystemExPrivatePCH.h";
#endif

		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});


		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});


		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
//				"CoreUObject",
//				"Engine"
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
