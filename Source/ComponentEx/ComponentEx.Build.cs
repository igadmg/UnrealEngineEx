using System.IO;
using UnrealBuildTool;



public class ComponentEx : ModuleRules
{
	public ComponentEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		bEnforceIWYU = true;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		PrivatePCHHeaderFile = "Private/ComponentExPrivatePCH.h";


		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});


		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});


		PublicDependencyModuleNames.AddRange(new string[] {
				"AIModule",
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"CoordinateSystemEx",
				"CoreEx",
				"InterpolationEx",
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
