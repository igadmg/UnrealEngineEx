using UnrealBuildTool;



public class UnrealEditorEx : ModuleRules
{
	public UnrealEditorEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PublicIncludePaths.AddRange(new string[] {
				"UnrealEditorEx/Public"
			});


		PrivateIncludePaths.AddRange(new string[] {
				"UnrealEditorEx/Private",
			});


		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"InputCore",
				"UnrealEd",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"UnrealEngineEx",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"CoreUObject",
				"EditorStyle",
				"Engine",
				"PropertyEditor",
				"SlateCore",
				"Slate",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
