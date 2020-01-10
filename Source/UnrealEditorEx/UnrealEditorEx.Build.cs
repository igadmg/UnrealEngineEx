using System.IO;
using UnrealBuildTool;



public class UnrealEditorEx : ModuleRules
{
	public UnrealEditorEx(ReadOnlyTargetRules Target)
		: base(Target)
	{
#if UE_4_21_OR_LATER
		PrivatePCHHeaderFile = "Private/UnrealEditorExPrivatePCH.h";
#endif

		PublicIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			});


		PrivateIncludePaths.AddRange(new string[] {
				Path.Combine(ModuleDirectory, "Private"),
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
