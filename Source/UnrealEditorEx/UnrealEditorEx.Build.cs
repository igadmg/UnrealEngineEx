using System.IO;
using UnrealBuildTool;



public class UnrealEditorEx : ModuleRules
{
	public UnrealEditorEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Private/UnrealEditorExPrivatePCH.h";

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
				"InputCore",
				"UnrealEd",
			});

		PublicDependencyModuleNames.AddRange(new string[] {
				"UnrealEngineEx",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"EditorStyle",
				"PropertyEditor",
				"SlateCore",
				"Slate",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
