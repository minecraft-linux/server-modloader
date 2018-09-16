#include <modloader/hook.h>
#include <modloader/log.h>

extern "C" {
#include <funchook.h>
}

using namespace modloader;

extern "C" {

modloader_hook_t *modloader_hook(void *sym, void *hook, void **orig) {
    funchook_t *h = funchook_create();
    if (!h)
        return nullptr;
    *orig = sym;
    int r = funchook_prepare(h, orig, hook);
    if (r != 0) {
        Log::error("Hook", "Error while hooking: %s", funchook_error_message(h));
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