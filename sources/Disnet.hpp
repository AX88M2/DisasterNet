#pragma once

#if defined(_WIN32)
    #define export_func(TYPE) extern "C" TYPE __declspec(dllexport)
#elif defined(__GNUC__)
    #define export_func(TYPE) extern "C" TYPE __attribute__((visibility("default")))
#else
    #define export_func(TYPE)
#endif

export_func(double) disnet_init();
export_func(void) disnet_uninit();
export_func(void) disnet_reset();
export_func(double) disnet_connect(const char *ip, double port);
export_func(double) disnet_poll(YYRValue *data);
export_func(void) disnet_send(const void *data, double size, double reliable);

