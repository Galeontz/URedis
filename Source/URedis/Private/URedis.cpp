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

#pragma region Pub/Sub

uint64 FURedis::Publish(FStringView channel, FStringView message) const {
    return _instance->publish(TCHAR_TO_UTF8(channel.GetData()),
                              TCHAR_TO_UTF8(message.GetData()));
}

void FURedis::Subscribe(FStringView channel,
               void (*MsgCallback)(FStringView channel, FStringView message)) {
    _instance->subscriber().subscribe(TCHAR_TO_UTF8(channel.GetData()));
    _instance->subscriber().on_message(MsgCallback);
}

void FURedis::Unsubscribe() const { _instance->subscriber().unsubscribe(); }

void FURedis::Unsubscribe(FStringView channel) const {
    _instance->subscriber().unsubscribe(TCHAR_TO_UTF8(channel.GetData()));
}

#pragma endregion

IMPLEMENT_MODULE(FURedis, URedis);