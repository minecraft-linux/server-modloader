#include <modloader/hook.h>
#include <modloader/log.h>

extern "C" {
#include <funchook.h>
}

using namespace modloader;

extern "C" {

void *modloader_dlsym_print_error(const char *sym) {
    void *ptr = dlsym(RTLD_DEFAULT, sym);
    if (!ptr)
        Log::error("Hook", "Unknown symbol %s", sym);
    return ptr;
}

modloader_hook_t *modloader_hook(void *sym, void *hook, void **orig) {
    funchook_t *h = funchook_create();
    if (!h)
        return nullptr;
    *orig = sym;
    int r = funchook_prepare(h, orig, hook);
    if (r != 0) {
        Log::error("Hook", "Error while preparing hook: %s", funchook_error_message(h));
        funchook_destroy(h);
        return nullptr;
    }
    r = funchook_install(h, 0);
    if (r != 0) {
        Log::error("Hook", "Error while installing hook: %s", funchook_error_message(h));
        modloader_destroy_hook(h);
        return nullptr;
    }
    return (modloader_hook_t *) h;
}

void modloader_destroy_hook(modloader_hook_t *hook) {
    auto h = (funchook_t *) hook;
    int r = funchook_uninstall(h, 0);
    if (r != 0)
        Log::error("Hook", "Error while uninstalling hook: %s", funchook_error_message(h));
    r = funchook_destroy(h);
    if (r != 0)
        Log::error("Hook", "Error while destroying hook: %s", funchook_error_message(h));
}

}