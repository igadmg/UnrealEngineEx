using System.IO;
using UnrealBuildTool;



public class UnrealEditorEx : ModuleRules
{
	public UnrealEditorEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
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
				"UMG",
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
				"MathEx"
			});


		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
