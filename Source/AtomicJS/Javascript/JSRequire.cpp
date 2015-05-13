
#include <assert.h>

#include <Atomic/IO/FileSystem.h>
#include <Atomic/Resource/ResourceCache.h>

#include "JSVM.h"
#include "JSRequire.h"
#include "JSPlugin.h"

namespace Atomic
{
    
    // see http://duktape.org/guide.html#modules   
    static int js_module_search(duk_context* ctx)
    {       
        JSVM* vm = JSVM::GetJSVM(ctx);
        FileSystem* fs = vm->GetSubsystem<FileSystem>();
        ResourceCache* cache = vm->GetSubsystem<ResourceCache>();

        int top = duk_get_top(ctx);

        assert(top ==  4);

        String moduleID = duk_to_string(ctx, 0);

        if (top > 1)
        {
            // require function
            assert(duk_is_function(ctx, 1));
        }
        
        if (top > 2)
        {
            // exports
            assert(duk_is_object(ctx, 2));
        }

        if (top > 3)        
        {
            // module (module.id == a resolved absolute identifier for the module being loaded)
            assert(duk_is_object(ctx, 3));
        }

        String pathName, fileName, extension;
        SplitPath(moduleID, pathName, fileName, extension);
        String path = moduleID;

        // Do we really want this?  It is nice to not have to specify the Atomic path
        if (fileName.StartsWith("Atomic"))
        {
            path = "AtomicModules/" + path + ".js";
        }
        else
        {
            path += ".js";
 
        }

        SharedPtr<File> jsfile(cache->GetFile(path, false));

        if (!jsfile)
        {
            // we're not a JS file, so check if we're a native module
            const Vector<String>& resourceDirs = cache->GetResourceDirs();

            for (unsigned i = 0; i < resourceDirs.Size(); i++)
            {

#ifdef ATOMIC_PLATFORM_WINDOWS

                // TODO: proper platform folder detection

                String pluginLibrary = resourceDirs.At(i) + "Plugins/Windows/x64/" + moduleID + ".dll";

                if (fs->FileExists(pluginLibrary))
                {
                    // let duktape know we loaded a native module
                    if (jsplugin_load(vm, pluginLibrary))
                    {
                        duk_push_undefined(ctx);
                        return 1;
                    }
                }

            }

#endif
        }
        else
        {
            vm->SetLastModuleSearchFile(jsfile->GetFullPath());
            String source;
            jsfile->ReadText(source);
            duk_push_string(ctx, source.CString());
        }

        return 1;

    }

    void js_init_require(JSVM* vm)
    {
        duk_context* ctx = vm->GetJSContext();
        duk_get_global_string(ctx, "Duktape");
        duk_push_c_function(ctx, js_module_search, 4);
        duk_put_prop_string(ctx, -2, "modSearch");
        duk_pop(ctx);
    }

}