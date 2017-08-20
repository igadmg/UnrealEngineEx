using UnrealBuildTool;



public class UnrealEngineEx : ModuleRules
{
	public UnrealEngineEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PublicIncludePaths.AddRange(new string[] {
				"UnrealEngineEx/Public"
			});


		PrivateIncludePaths.AddRange(new string[] {
				"UnrealEngineEx/Private",
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

		if (UEBuildConfiguration.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
					"UnrealEd",
				});
		}
	}
}
