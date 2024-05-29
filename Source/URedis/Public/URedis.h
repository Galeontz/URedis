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

    FString Ping(TOptional<FString> message = NullOpt) const;

    /**
     * @brief Sets the value for a given key.
     *
     * This method assigns a value to the specified key. If the key already
     * exists, its value will be updated. This operation is performed in a
     * constant manner, meaning that the object itself remains logically
     * immutable.
     *
     * @param key The key to which the value is to be assigned. This parameter
     * is a constant reference to an FStringView object representing the key.
     * @param value The value to be assigned to the key. This parameter is a
     * constant reference to an FStringView object representing the value.
     * @return True if the value was successfully set; otherwise, false.
     */
    bool Set(FStringView key, FStringView value) const;

    /**
     * @brief Retrieves the value associated with the given key.
     *
     * This method searches for the specified key and returns the corresponding
     * value if the key is found. If the key is not found, it returns an empty
     * optional.
     *
     * @param key The key for which the value is to be retrieved. This parameter
     * is a constant reference to an FStringView object representing the key.
     * @return An optional containing the value associated with the key if the
     * key is found; otherwise, an empty optional.
     */
    TOptional<FString> Get(FStringView key) const;

private:
    TUniquePtr<sw::redis::Redis> _instance{};
};
