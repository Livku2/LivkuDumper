#include <dlfcn.h>

#define _Tag "LivkuDumper"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, _Tag, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, _Tag, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, _Tag, __VA_ARGS__)

#define sym dlsym
#define open dlopen