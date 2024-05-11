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
    check(_instance);
}

FString FURedis::Ping(TOptional<FString> message) const {
    check(_instance);
    std::string reply{};
    if (message) {
        reply = _instance->echo(TCHAR_TO_UTF8(**message));
    } else {
        reply = _instance->ping();
    }
    return UTF8_TO_TCHAR(reply.c_str());
}

IMPLEMENT_MODULE(FURedis, URedis);