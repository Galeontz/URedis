#include "URedis.h"

#include "HAL/Platform.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/WindowsHWrapper.h"
#endif

THIRD_PARTY_INCLUDES_START
#include "sw/redis++/redis.h"
THIRD_PARTY_INCLUDES_END

#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#include "Modules/ModuleManager.h"

#include <string>

void FURedis::Connect(const FStringView host, const int32 port) {
    sw::redis::ConnectionOptions opts;
    opts.host = TCHAR_TO_UTF8(host.GetData());
    opts.port = port;
    _instance = MakeUnique<sw::redis::Redis>(opts);
    check(Ping() == "PONG");
}

FString FURedis::Ping(TOptional<FString> message) const {
    std::string reply{};
    if (message) {
        reply = _instance->echo(TCHAR_TO_UTF8(**message));
    } else {
        reply = _instance->ping();
    }
    return UTF8_TO_TCHAR(reply.c_str());
}

bool FURedis::Set(const FStringView key, const FStringView value) const {
    return _instance->set(TCHAR_TO_UTF8(key.GetData()),
                          TCHAR_TO_UTF8(value.GetData()));
}

TOptional<FString> FURedis::Get(const FStringView key) const {
    const auto result{_instance->get(TCHAR_TO_UTF8(key.GetData()))};

    if (!result) {
        return NullOpt;
    }

    return {UTF8_TO_TCHAR(result->c_str())};
}

uint64 FURedis::Del(const FStringView key) const {
    return _instance->del(TCHAR_TO_UTF8(key.GetData()));
}

void FURedis::Rename(const FStringView key, const FStringView newKey) const {
    _instance->rename(TCHAR_TO_UTF8(key.GetData()),
                      TCHAR_TO_UTF8(newKey.GetData()));
}

#pragma region Geostapial functions

uint64 FURedis::GeoAdd(const FStringView key,
                       TTuple<FStringView, double, double> geoData) const {
    const auto &member = geoData.Get<0>();
    const auto longitude = geoData.Get<1>();
    const auto latitude = geoData.Get<2>();

    const auto convertedGeoData =
        std::tuple<sw::redis::StringView, double, double>(
            TCHAR_TO_UTF8(member.GetData()), longitude, latitude);

    return _instance->geoadd(TCHAR_TO_UTF8(key.GetData()), convertedGeoData);
}

TOptional<double> FURedis::GeoDistance(const FStringView key,
                                       const FStringView member1,
                                       const FStringView member2,
                                       const GeoUnit unit) const {
    const auto result{_instance->geodist(
        TCHAR_TO_UTF8(key.GetData()), TCHAR_TO_UTF8(member1.GetData()),
        TCHAR_TO_UTF8(member2.GetData()),
        static_cast<sw::redis::GeoUnit>(unit))};

    if (!result) {
        return NullOpt;
    }

    return *result;
}

TOptional<TPair<double, double>> FURedis::GeoPosition(
    const FStringView key, const FStringView member) const {
    const auto result{_instance->geopos(TCHAR_TO_UTF8(key.GetData()),
                                        TCHAR_TO_UTF8(member.GetData()))};

    if (!result) {
        return NullOpt;
    }

    return TPair<double, double>(result->first, result->second);
}

TOptional<uint64> FURedis::GeoRadius(const FStringView key,
                                     const TPair<double, double> location,
                                     const double radius, const GeoUnit unit,
                                     const FStringView destination,
                                     const bool storeDist,
                                     const uint64 count) const {
    const auto result{_instance->georadius(
        TCHAR_TO_UTF8(key.GetData()), std::pair(location.Key, location.Value),
        radius, static_cast<sw::redis::GeoUnit>(unit),
        TCHAR_TO_UTF8(destination.GetData()), storeDist, count)};

    if (!result) {
        return NullOpt;
    }

    return *result;
}

TOptional<uint64> FURedis::GeoRadiusByMember(
    const FStringView key, const FStringView member, const double radius,
    const GeoUnit unit, const FStringView destination, const bool storeDist,
    const uint64 count) const {
    const auto result{_instance->georadiusbymember(
        TCHAR_TO_UTF8(key.GetData()), TCHAR_TO_UTF8(member.GetData()), radius,
        static_cast<sw::redis::GeoUnit>(unit),
        TCHAR_TO_UTF8(destination.GetData()), storeDist, count)};

    if (!result) {
        return NullOpt;
    }

    return *result;
}

#pragma endregion

IMPLEMENT_MODULE(FURedis, URedis);