using System.IO;
using System.Diagnostics;
using UnrealBuildTool;



public class InterpolationEx : ModuleRules
{
	public InterpolationEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivateIncludePaths.AddRange(new string[] {
				"InterpolationEx/Private"
			});

		PublicIncludePaths.AddRange(new string[] {
				"InterpolationEx/Public"
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

