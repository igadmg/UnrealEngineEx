using System.IO;
using UnrealBuildTool;



public class MathEx : ModuleRules
{
	public MathEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		PrivatePCHHeaderFile = "Private/MathExPrivatePCH.h";


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
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
