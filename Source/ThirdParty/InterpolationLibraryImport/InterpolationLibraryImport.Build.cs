using System;
using System.IO;
using UnrealBuildTool;



public class InterpolationLibraryImport : ModuleRules
{
	public InterpolationLibraryImport(ReadOnlyTargetRules Target)
		: base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "InterpolationLibrary", "src")));
	}
}
