#pragma once

#if defined(_WIN32)
    #define export_sym(TYPE) extern "C" TYPE __declspec(dllexport)
#elif defined(__GNUC__)
    #define export_func(TYPE) extern "C" TYPE __attribute__((visibility("default")))
#else
    #define export_func(TYPE)
#endif

export_sym(double) disnet_init();
export_sym(void) disnet_uninit();
export_sym(void) disnet_reset();
export_sym(double) disnet_connect(const char *ip, double port);
export_sym(double) disnet_poll(char *buffer);
export_sym(void) disnet_send(char *data, double length, double reliable);

