using System.IO;
using UnrealBuildTool;



public class UnrealEngineEx : ModuleRules
{
	public UnrealEngineEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
#if UE_4_21_OR_LATER
#if UE_4_24_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.V2;
#endif
		PrivatePCHHeaderFile = "Private/UnrealEngineExPrivatePCH.h";
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
				"CoordinateSystemEx",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"EngineSettings",
				"InputCore",
				"SlateCore",
				"Slate",
				"UMG",
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
