#ifndef MODLOADER_HOOK_H
#define MODLOADER_HOOK_H

#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void modloader_hook_t;

void* modloader_dlsym_print_error(const char *sym);

modloader_hook_t* modloader_hook(void *sym, void *hook, void **orig);

void modloader_destroy_hook(modloader_hook_t*);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

namespace modloader {

class AutoHook {

private:
    modloader_hook_t* hook;

    template <typename T>
    static void *castToVoid(T hook) {
        union {
            T a;
            void *b;
        } hookUnion;
        hookUnion.a = hook;
        return hookUnion.b;
    }

public:
    AutoHook(void *sym, void *hook, void **orig) {
        this->hook = modloader_hook(sym, hook, orig);
    }

    ~AutoHook() {
        if (hook)
            modloader_destroy_hook(hook);
    }


    AutoHook(const char *sym, void *hook, void **orig) : AutoHook(modloader_dlsym_print_error(sym), hook, orig) {
    }

    // workaround for a warning
    template<typename T>
    AutoHook(const char *sym, T hook, void **orig) : AutoHook(sym, castToVoid(hook), orig) {
    }

};

}

#endif

#endif