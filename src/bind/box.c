
#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <string.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_box_class_id;

JSValue js_createBoxStyleFuncWithOpaque(JSContext *ctx, box_style_t *style);

static JSValue js_get_style(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);
    if (!box)
        return JS_EXCEPTION;
    uint32_t v;
    if (JS_ToUint32(ctx, &v, argv[0]))
        return JS_EXCEPTION;

    box_style_t *style = box_get_style(box, v);

    if (style)
        return js_createBoxStyleFuncWithOpaque(ctx, box_get_style(box, v));
    else
        return JS_NULL;
}

extern JSClassID get_js_box_style_class_id();

static JSValue js_set_style(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    box_style_t *style = JS_GetOpaque2(ctx, argv[0], get_js_box_style_class_id());
    if (!style)
        return JS_EXCEPTION;

    uint32_t v;
    if (JS_ToUint32(ctx, &v, argv[1]))
        return JS_EXCEPTION;

    box_set_style(box, style, v);
    return JS_UNDEFINED;
}

static JSValue js_add_child(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    box_t child = JS_GetOpaque2(ctx, argv[0], js_box_class_id);
    if (!child)
        return JS_EXCEPTION;

    Flex_addChild(box, child);
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_box_proto_funcs[] = {
    JS_CFUNC_DEF("getStyle", 1, js_get_style),
    JS_CFUNC_DEF("setStyle", 2, js_set_style),
    JS_CFUNC_DEF("addChild", 1, js_add_child),
};

static JSValue js_box_ctor(JSContext *ctx,
                           JSValueConst new_target,
                           int argc, JSValueConst *argv)
{
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, js_box_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    box_t box = box_new();

    JS_SetOpaque(obj, box);
    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

static void js_box_finalizer(JSRuntime *rt, JSValue val)
{
    struct box_t *box = JS_GetOpaque(val, js_box_class_id);
}

static JSClassDef js_box_class = {
    "Box",
    .finalizer = js_box_finalizer,
};

JSClassID get_js_box_class_id()
{
    return js_box_class_id;
}

static const JSCFunctionListEntry box_state_entries[] = {
    JS_PROP_INT32_DEF("DEFAULT", BOX_STATE_DEFAULT, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("FOCUS", BOX_STATE_FOCUS, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("ACTIVE", BOX_STATE_ACTIVE, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("HOVER", BOX_STATE_HOVER, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("SELECT", BOX_STATE_SELECT, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("DISABLE", BOX_STATE_DISABLE, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("CHECK", BOX_STATE_CHECK, JS_PROP_CONFIGURABLE),
};

int js_box_class_define(JSContext *ctx, JSModuleDef *m)
{
    JS_NewClassID(&js_box_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_box_class_id, &js_box_class);

    JSValue box_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, box_proto, js_box_proto_funcs, countof(js_box_proto_funcs));
    JSValue box_class = JS_NewCFunction2(ctx, js_box_ctor, "Box", 0, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, box_class, box_proto);
    JS_SetClassProto(ctx, js_box_class_id, box_proto);
    JS_SetModuleExport(ctx, m, "Box", box_class);

    JSValue js_box_state = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, js_box_state, box_state_entries, countof(box_state_entries));
    JS_SetModuleExport(ctx, m, "BOX_STATE", js_box_state);
    return 0;
}