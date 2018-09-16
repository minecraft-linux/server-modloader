#ifndef MODLOADER_HOOK_H
#define MODLOADER_HOOK_H

#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void modloader_hook_t;

modloader_hook_t* modloader_hook(void *sym, void *hook, void **orig);

void modloader_destroy_hook(modloader_hook_t*);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

namespace modloader {

struct AutoHook {

    modloader_hook_t* hook;

    AutoHook(void *sym, void *hook, void **orig) {
        this->hook = modloader_hook(sym, hook, orig);
    }

    ~AutoHook() {
        if (hook)
            modloader_destroy_hook(hook);
    }


    AutoHook(const char *sym, void *hook, void **orig) : AutoHook(dlsym(RTLD_DEFAULT, sym), hook, orig) {
    }

    // workaround for a warning
    template<typename T>
    AutoHook(const char *sym, T hook, void **orig) {
        union {
            T a;
            void *b;
        } hookUnion;
        hookUnion.a = hook;
        AutoHook(sym, hookUnion.b, orig);
    }

};

}

#endif

#endif