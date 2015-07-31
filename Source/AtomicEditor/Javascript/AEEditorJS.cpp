
#include <AtomicJS/Javascript/JSVM.h>

#include "../Application/AEPreferences.h"

#include "../EditorMode/AEEditorMode.h"

using namespace Atomic;

namespace Atomic
{
    extern void jsb_package_editor_init(JSVM* vm);
}

namespace AtomicEditor
{

void jsapi_init_editor(JSVM* vm)
{
    duk_context* ctx = vm->GetJSContext();

    duk_push_object(ctx);
    duk_put_global_string(ctx, "Editor");

    jsb_package_editor_init(vm);

    duk_get_global_string(ctx, "Atomic");

    if (vm->GetContext()->GetEditorContext()) {
        js_push_class_object_instance(ctx, vm->GetSubsystem<EditorMode>(), "EditorMode");
        duk_put_prop_string(ctx, -2, "editorMode");
    }

    duk_pop(ctx);

}

}


