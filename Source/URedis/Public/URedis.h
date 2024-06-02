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
     * is an FStringView representing the key.
     * @param value The value to be assigned to the key. This parameter is a
     * FStringView object representing the value.
     * @return True if the value was successfully set; otherwise, false.
     */
    bool Set(FStringView key, FStringView value) const;

    /**
     * @brief Retrieves the value associated with the given key.
     *
     * This method searches for the specified key and returns the corresponding
     * value if the key is found. If the key is not found, it returns NullOpt.
     *
     * @param key The key for which the value is to be retrieved. This parameter
     * is an FStringView representing the key.
     * @return An optional containing the value associated with the key if the
     * key is found; otherwise, NullOpt.
     */
    TOptional<FString> Get(FStringView key) const;

    /**
     * @brief Deletes the specified key.
     *
     * This method removes the specified key from the Redis database. If the key
     * is found and deleted, it returns the number of keys that were removed.
     *
     * @param key The key to be deleted. This parameter is an FStringView
     * representing the key.
     * @return The number of keys that were removed.
     */
    uint64 Del(FStringView key) const;

    /**
     * @brief Renames the specified key.
     *
     * This method changes the name of the specified key to the new name. If the
     * key is found and renamed, the method returns void.
     *
     * @param key The key to be renamed. This parameter is an FStringView
     * representing the key.
     * @param newKey The new name for the key. This parameter is an FStringView
     * representing the new key name.
     */
    void Rename(FStringView key, FStringView newKey) const;

private:
    TUniquePtr<sw::redis::Redis> _instance{};
};
