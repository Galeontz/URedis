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

bool FURedis::Set(FStringView key, FStringView value) const {
    return _instance->set(TCHAR_TO_UTF8(key.GetData()),
                          TCHAR_TO_UTF8(value.GetData()));
}

TOptional<FString> FURedis::Get(FStringView key) const {
    auto result{_instance->get(TCHAR_TO_UTF8(key.GetData()))};

    if (!result) {
        return NullOpt;
    }

    return {UTF8_TO_TCHAR(result->c_str())};
}

uint64 FURedis::Del(FStringView key) const {
    return _instance->del(TCHAR_TO_UTF8(key.GetData()));
}

void FURedis::Rename(FStringView key, FStringView newKey) const {
    _instance->rename(TCHAR_TO_UTF8(key.GetData()),
                      TCHAR_TO_UTF8(newKey.GetData()));
}

#pragma region Geostapial functions

uint64 FURedis::GeoAdd(FStringView key,
                       TTuple<FStringView, double, double> geoData) const {
    const auto &member = geoData.Get<0>();
    const auto longitude = geoData.Get<1>();
    const auto latitude = geoData.Get<2>();

    const auto convertedGeoData =
        std::tuple<sw::redis::StringView, double, double>(
            TCHAR_TO_UTF8(member.GetData()), longitude, latitude);

    return _instance->geoadd(TCHAR_TO_UTF8(key.GetData()), convertedGeoData);
}

TOptional<double> FURedis::GeoDistance(FStringView key, FStringView member1,
                                       FStringView member2) const {
    auto result{_instance->geodist(TCHAR_TO_UTF8(key.GetData()),
                                   TCHAR_TO_UTF8(member1.GetData()),
                                   TCHAR_TO_UTF8(member2.GetData()))};

    if (!result) {
        return NullOpt;
    }

    return {result.value()};
}

TOptional<TPair<double, double>> FURedis::GeoPosition(
    FStringView key, FStringView member) const {
    auto result{_instance->geopos(TCHAR_TO_UTF8(key.GetData()),
                                  TCHAR_TO_UTF8(member.GetData()))};

    if (!result) {
        return NullOpt;
    }

    return TOptional<TPair<double, double>>(
        TPair<double, double>(result.value().first, result.value().second));
}

#pragma endregion

IMPLEMENT_MODULE(FURedis, URedis);