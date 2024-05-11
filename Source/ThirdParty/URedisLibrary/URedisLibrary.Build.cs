using System.IO;
using UnrealBuildTool;

public class URedisLibrary : ModuleRules {
  public URedisLibrary(ReadOnlyTargetRules Target) : base(Target) {
    Type = ModuleType.External;

    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

    if (Target.Platform == UnrealTargetPlatform.Linux) {
      var libname = "libredis++.so";
      var path = Path.Combine(ModuleDirectory, "linux", "x64", libname);
      PublicAdditionalLibraries.Add(path);
      RuntimeDependencies.Add(path);

      libname = "libhiredis.so";
      path = Path.Combine(ModuleDirectory, "linux", "x64", libname);
      PublicAdditionalLibraries.Add(path);
      RuntimeDependencies.Add(path);
    }
  }
}
