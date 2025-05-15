#ifndef POKMONOS_H
#define POKMONOS_H



namespace PokmonOS{
    using u8 = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;

    using s8 = signed char;
    using s16 = signed short;
    using s32 = signed int;
    using s64 = signed long long;


    using size_t = unsigned long;
    using intptr_t = long;

    enum class SystemStatus{
        INITIALIZING,
        RUNNING,
        ERROR,
        SHUTDOWN
    };

    enum class ErrorCode{
        SUCCESS = 0,
        GENERAL_ERROR = 1,
        OUT_OF_MEMORY = 2,
        INVALID_PARAMETER = 3
    };

    void initializeSystem();
    void shutdownSystem();
    void panic(const char* message);
}
#endif //POKMONOS_H