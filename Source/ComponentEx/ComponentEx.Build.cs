using UnrealBuildTool;



public class ComponentEx : ModuleRules
{
	public ComponentEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PublicIncludePaths.AddRange(new string[] {
				"ComponentEx/Public"
			});


		PrivateIncludePaths.AddRange(new string[] {
				"ComponentEx/Private",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
