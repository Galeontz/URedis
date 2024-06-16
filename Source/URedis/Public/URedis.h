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

#pragma region Geospatial functions

    /**
     * @brief Enum class representing units of geographical distance.
     *
     * This enum class defines the various units of measurement that can be used
     * to specify distances in geographical contexts. It includes meters (M),
     * kilometers (KM), miles (MI), and feet (FT).
     */
    enum class GeoUnit { M, KM, MI, FT };

    /**
     * @brief Adds a geographical location to a specified key.
     *
     * This function adds a geographical location specified by its name and
     * coordinates (longitude and latitude) to a key.
     * The location is defined using a tuple containing the
     * name of the location, the longitude, and the latitude.
     *
     * @param key The key to which the geographical location will be added. This
     * parameter is a constant reference to an FStringView object representing
     * the key.
     * @param geoData A tuple containing the geographical data:
     *                - The name of the location (FStringView).
     *                - The longitude of the location (double).
     *                - The latitude of the location (double).
     * @return The number of elements that were added to the sorted set, not
     * including all the elements already existing for which the score was
     * updated.
     */
    uint64 GeoAdd(FStringView key,
                  TTuple<FStringView, double, double> geoData) const;

    /**
     * @brief Calculates the distance between two geographical members.
     *
     * This function computes the distance between two members stored in a
     * specified key, returning the distance in the specified unit of
     * measurement. The members and the unit are specified as parameters.
     *
     * @param key The key containing the geographical members. This parameter is
     * a constant reference to an FStringView object representing the key.
     * @param member1 The first member whose geographical position will be used
     * in the distance calculation. This parameter is a constant
     * reference to an FStringView object representing the first member.
     * @param member2 The second member whose geographical position will be used
     in the distance calculation. This parameter is a constant
     * reference to an FStringView object representing the second member.
     * @param unit The unit of measurement for the distance. This parameter is
     * of type GeoUnit and can be meters (M), kilometers (KM), miles
     * (MI), or feet (FT).
     * @return An optional containing the distance between the two members if
     * both members exist; otherwise, an empty optional.
     */
    TOptional<double> GeoDistance(FStringView key, FStringView member1,
                                  FStringView member2, GeoUnit unit) const;

    /**
     * @brief Retrieves the geographical position of a member.
     *
     * This function returns the longitude and latitude of a specified member
     * stored in a given key. If the member exists, the function returns a pair
     * containing the longitude and latitude. If the member does not exist, it
     * returns an empty optional.
     *
     * @param key The key containing the geographical member. This parameter is
     * a constant reference to an FStringView object representing the key.
     * @param member The member whose geographical position is to be retrieved.
     * This parameter is a constant reference to an FStringView
     * object representing the member.
     * @return An optional containing a pair of doubles representing the
     * longitude and latitude of the member if the member exists;
     * otherwise, an empty optional.
     */
    TOptional<TPair<double, double>> GeoPosition(FStringView key,
                                                 FStringView member) const;

    /**
     * @brief Finds members within a radius from a specified geographical point.
     *
     * This function returns the number of members within a specified radius
     * from a given geographical location (longitude and latitude) stored in a
     * key. The results can be stored in a destination key, and optionally the
     * distances from the central point to each member can also be stored.
     *
     * @param key The key containing the geographical members. This parameter is
     * a constant reference to an FStringView object representing the key.
     * @param location A pair of doubles representing the longitude and latitude
     * of the central point.
     * @param radius The radius around the central point within which to find
     * members.
     * @param unit The unit of measurement for the radius. This parameter is of
     * type GeoUnit and can be meters (M), kilometers (KM), miles (MI), or feet
     * (FT).
     * @param destination The key where the results will be stored. This
     * parameter is a constant reference to an FStringView object representing
     * the destination key.
     * @param storeDist A boolean flag indicating whether to store the
     * distances from the central point to each member.
     * @param count The maximum number of members to return.
     * @return An optional containing the number of members found within the
     * radius if the operation is successful; otherwise, an empty optional.
     */
    TOptional<uint64> GeoRadius(FStringView key, TPair<double, double> location,
                                double radius, GeoUnit unit,
                                FStringView destination, bool storeDist,
                                uint64 count) const;

    /**
     * @brief Finds members within a radius from a specified member.
     *
     * This function returns the number of members within a specified radius
     * from a given member stored in a key. The results can be stored in a
     * destination key, and optionally the distances from the specified member
     * to each found member can also be stored.
     *
     * @param key The key containing the geographical members. This parameter is
     * a constant reference to an FStringView object representing the key.
     * @param member The member from which the radius is calculated. This
     * parameter is a constant reference to an FStringView object representing
     * the member.
     * @param radius The radius around the specified member within which to find
     * other members.
     * @param unit The unit of measurement for the radius. This parameter is of
     * type GeoUnit and can be meters (M), kilometers (KM), miles (MI), or feet
     * (FT).
     * @param destination The key where the results will be stored. This
     * parameter is a constant reference to an FStringView object representing
     * the destination key.
     * @param storeDist A boolean flag indicating whether to store the
     * distances from the specified member to each found member.
     * @param count The maximum number of members to return.
     * @return An optional containing the number of members found within the
     * radius if the operation is successful; otherwise, an empty optional.
     */
    TOptional<uint64> GeoRadiusByMember(FStringView key, FStringView member,
                                        double radius, GeoUnit unit,
                                        FStringView destination, bool storeDist,
                                        uint64 count) const;

#pragma endregion

private:
    TUniquePtr<sw::redis::Redis> _instance{};
};
