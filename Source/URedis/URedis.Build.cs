using UnrealBuildTool;

public class URedis : ModuleRules {
  public URedis(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    DefaultBuildSettings = BuildSettingsVersion.V5;

    PublicDependencyModuleNames.AddRange(new[] {
      "Core",
      "URedisLibrary",
    });
  }
}
