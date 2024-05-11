#pragma once

#include "Containers/UnrealString.h"
#include "HAL/Platform.h"
#include "Modules/ModuleManager.h"
#include "Templates/UniquePtr.h"

// Forward declare private type in the public header
namespace sw::redis {
class Redis;
}

class UREDIS_API FURedis final : public IModuleInterface {
  public:
  // There's no need to override IModuleInterface
  // StartupModule & ShutdownModule methods.
  
  void Connect(const FStringView host, int32 port = 6379);

  FString Ping(TOptional<FString> message) const;

  private:
  TUniquePtr<sw::redis::Redis> _instance{};
};
