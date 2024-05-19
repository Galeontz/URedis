using System.IO;
using UnrealBuildTool;

public class URedisLibrary : ModuleRules {
  public URedisLibrary(ReadOnlyTargetRules target) : base(target) {
    Type = ModuleType.External;

    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

    if (target.Platform == UnrealTargetPlatform.Linux) {
      // hiredis
      var path = Path.Combine(ModuleDirectory, "linux", "x64", "libredis++.so");
      PublicAdditionalLibraries.Add(path);
      RuntimeDependencies.Add(path);

      // redis++
      path = Path.Combine(ModuleDirectory, "linux", "x64", "libhiredis.so");
      PublicAdditionalLibraries.Add(path);
      RuntimeDependencies.Add(path);
    } else if (target.Platform == UnrealTargetPlatform.Win64) {
      // hiredis
      PublicAdditionalLibraries.Add(
          Path.Combine(ModuleDirectory, "windows", "x64", "hiredis.lib"));
      RuntimeDependencies.Add(
          Path.Combine(PluginDirectory, "Binaries", "Win64", "hiredis.dll"));
      // redis++
      PublicAdditionalLibraries.Add(
          Path.Combine(ModuleDirectory, "windows", "x64", "redis++.lib"));
      RuntimeDependencies.Add(
          Path.Combine(PluginDirectory, "Binaries", "Win64", "redis++.dll"));
    }
  }
}
