using System.IO;
using UnrealBuildTool;

public class URedisLibrary : ModuleRules {
  public URedisLibrary(ReadOnlyTargetRules Target) : base(Target) {
    Type = ModuleType.External;

    if (Target.Platform == UnrealTargetPlatform.Linux) {
      PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include", "linux"));

      var libname = "libredis++.so";
      var path = Path.Combine(ModuleDirectory, "linux", "x64", libname);
      PublicAdditionalLibraries.Add(path);
      RuntimeDependencies.Add(path);

      libname = "libhiredis.so";
      path = Path.Combine(ModuleDirectory, "linux", "x64", libname);
      PublicAdditionalLibraries.Add(path);
      RuntimeDependencies.Add(path);
    } else if (Target.Platform == UnrealTargetPlatform.Win64) {
      PublicIncludePaths.Add(
          Path.Combine(ModuleDirectory, "include", "windows"));

      var libName = "hiredis.lib";
      var dllName = "hiredis.dll";

      PublicAdditionalLibraries.Add(
          Path.Combine(ModuleDirectory, "windows", "x64", libName));
      RuntimeDependencies.Add(
          Path.Combine(PluginDirectory, "Binaries", "Win64", dllName));

      libName = "redis++.lib";
      dllName = "redis++.dll";

      PublicAdditionalLibraries.Add(
          Path.Combine(ModuleDirectory, "windows", "x64", libName));
      RuntimeDependencies.Add(
          Path.Combine(PluginDirectory, "Binaries", "Win64", dllName));
    }
  }
}
