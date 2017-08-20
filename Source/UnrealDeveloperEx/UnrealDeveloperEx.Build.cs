using UnrealBuildTool;



public class UnrealDeveloperEx : ModuleRules
{
	public UnrealDeveloperEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PublicIncludePaths.AddRange(new string[] {
				"UnrealDeveloperEx/Public"
			});


		PrivateIncludePaths.AddRange(new string[] {
				"UnrealDeveloperEx/Private",
			});


		PublicDependencyModuleNames.AddRange(new string[] {
				"BlueprintGraph",
				"Core",
				"UnrealEd",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"CoreUObject",
				"Engine",
				"KismetCompiler",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
				"StateMachineEx",
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
