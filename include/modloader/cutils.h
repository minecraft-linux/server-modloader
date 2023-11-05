#ifndef MODLOADER_CUTILS_H
#define MODLOADER_CUTILS_H

#include <modloader/hook.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef RTLD_DEFAULT
#define RTLD_DEFAULT NULL
#endif

#define SYM(sym)                                            \
    dlsym(RTLD_DEFAULT, sym)


#define SYMCALL(sym, func_proto, ...)                       \
    ((func_proto)                                           \
    (SYM(sym)))                                             \
    (__VA_ARGS__)


#define VIRTUAL_CALL(ptr, func_proto, ...)                  \
    ((func_proto)                                           \
    ((void *)ptr))                                          \
    (__VA_ARGS__)


#define DEREFERENCE(type, ptr, offset)                      \
    (*(type*)((uintptr_t)ptr + offset))


#define REFERENCE(type, ptr, offset)                        \
    (type*)((uintptr_t)ptr + offset)


// for uintptr_t
#define PTR_OFFSET(ptr, offset)                             \
    ((uintptr_t)ptr + offset)


#define THOOK(name, ret_type, sym, ...)          		    \
    typedef ret_type (*_##name##_t)(__VA_ARGS__);           \
    ret_type _detour_##name(__VA_ARGS__);                   \
    void _install_##name(void) __attribute__((constructor));\
    void _destroy_##name(void);                             \
                                                            \
    struct _##name {                                        \
        modloader_hook_t *hook;                             \
        _##name##_t detour;                                 \
        _##name##_t original;                               \
        void (*install)(void);                              \
        void (*destroy)(void);                              \
    } name = {NULL, _detour_##name, NULL,                   \
                _install_##name, _destroy_##name};          \
                                                            \
    void _install_##name(void)                              \
    {                                                       \
        name.hook = modloader_hook(SYM(sym),                \
                                name.detour,                \
                                (void**)&name.original);    \
    }                                                       \
                                                            \
    void _destroy_##name(void)                              \
    {                                                       \
        modloader_destroy_hook(name.hook);                  \
    }                                                       \
                                                            \
    ret_type _detour_##name(__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif


// void *sstr = std::string::basic_string(const char *c_str)
void std_string_string(void **sstr, const char *c_str);

// std::string::c_str()
const char *std_string_c_str(void *sstr);

// std::string::~basic_string()
void std_string_destroy(void *sstr, bool should_free);

#ifdef __cplusplus
}
#endif

#endif
