
#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <meui.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_meui_class_id;
extern JSClassID get_js_box_class_id();

static JSValue js_get_instance(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_register_callback(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    return JS_UNDEFINED;
}

static JSValue js_render(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    box_t box = JS_GetOpaque(argv[0], get_js_box_class_id());
    meui_render(meui, box);
    return JS_UNDEFINED;
}

static JSValue js_main_loop(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    meui_main_loop(meui);
    return JS_TRUE;
}

static JSValue js_flush(JSContext *ctx, JSValueConst this_val,
                        int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    return JS_UNDEFINED;
}
static JSValue js_update(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    return JS_UNDEFINED;
}
static JSValue js_end(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    return JS_UNDEFINED;
}
static JSValue js_add_font_face(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, argv[0]);
    if (!s)
        return JS_EXCEPTION;
    meui_add_font_face(meui, s);
    return JS_UNDEFINED;
}

static void js_meui_finalizer(JSRuntime *rt, JSValue val)
{
    struct meui_t *meui = JS_GetOpaque(val, js_meui_class_id);
    /* Note: 's' can be NULL in case JS_SetOpaque() was not called */
    meui_end(meui);
}

static JSClassDef js_meui_class = {
    "MEUI",
    .finalizer = js_meui_finalizer,
};

static JSValue js_meui_ctor(JSContext *ctx,
                            JSValueConst new_target,
                            int argc, JSValueConst *argv)
{
    int32_t width, height;

    JSValue obj = JS_UNDEFINED;
    JSValue proto;

    if (JS_ToInt32(ctx, &width, argv[0]))
        goto fail;
    if (JS_ToInt32(ctx, &height, argv[1]))
        goto fail;
    /* using new_target to get the prototype is necessary when the
       class is extended. */
    proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    obj = JS_NewObjectProtoClass(ctx, proto, js_meui_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    struct meui_t *meui = meui_start(width, height);

    JS_SetOpaque(obj, meui);
    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

static const JSCFunctionListEntry js_meui_proto_funcs[] = {
    JS_CFUNC_DEF("registerCallback", 2, js_register_callback),
    JS_CFUNC_DEF("render", 1, js_render),
    JS_CFUNC_DEF("mainLoop", 0, js_main_loop),
    JS_CFUNC_DEF("flush", 0, js_flush),
    JS_CFUNC_DEF("update", 0, js_update),
    JS_CFUNC_DEF("addFontFace", 1, js_add_font_face),
};

static int js_meui_class_define(JSContext *ctx, JSModuleDef *m)
{
    JSValue meui_proto, meui_class;
    JS_NewClassID(&js_meui_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_meui_class_id, &js_meui_class);

    meui_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, meui_proto, js_meui_proto_funcs, countof(js_meui_proto_funcs));
    meui_class = JS_NewCFunction2(ctx, js_meui_ctor, "MEUI", 2, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, meui_class, meui_proto);
    JS_SetClassProto(ctx, js_meui_class_id, meui_proto);
    JS_SetModuleExport(ctx, m, "MEUI", meui_class);

    return 0;
}

extern int js_box_class_define(JSContext *ctx, JSModuleDef *m);
extern int js_box_style_class_define(JSContext *ctx, JSModuleDef *m);
static int js_meui_module_init(JSContext *ctx, JSModuleDef *m)
{
    js_meui_class_define(ctx, m);
    js_box_class_define(ctx, m);
    js_box_style_class_define(ctx, m);
    return 0;
}

JSModuleDef *js_init_module_meui(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_meui_module_init);
    if (!m)
        return NULL;
    JS_AddModuleExport(ctx, m, "MEUI");
    JS_AddModuleExport(ctx, m, "Box");
    JS_AddModuleExport(ctx, m, "createBoxStyle");
    JS_AddModuleExport(ctx, m, "BOX_STATE");
    return m;
}