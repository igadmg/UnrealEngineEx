using System.IO;
using System.Diagnostics;
using UnrealBuildTool;



public class InterpolationEx : ModuleRules
{
	public InterpolationEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
#if UE_4_21_OR_LATER
		PrivatePCHHeaderFile = "Public/InterpolationEx.h";
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
				"Engine",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"InterpolationLibraryImport"
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
				"InputCore",
				"UnrealEngineEx",
			});
	}
}

