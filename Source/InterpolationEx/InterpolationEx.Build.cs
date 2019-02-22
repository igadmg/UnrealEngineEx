using System.IO;
using System.Diagnostics;
using UnrealBuildTool;



public class InterpolationEx : ModuleRules
{
	public InterpolationEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Public/InterpolationEx.h";

		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});

		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"InterpolationLibraryImport"
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"UnrealEngineEx",
			});
	}
}

