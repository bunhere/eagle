/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <EWebKit.h>
#include <Elementary.h>
#include <browser/WebView.h>

static Ewk_View_Smart_Class _parent_sc = EWK_VIEW_SMART_CLASS_INIT_NULL;
typedef struct _View_Smart_Data View_Smart_Data;
struct _View_Smart_Data
{
   Ewk_View_Smart_Data base;
   WebView* cppInstance;
};

static inline WebView* toCpp(Ewk_View_Smart_Data* sd)
{
    return ((View_Smart_Data*)sd)->cppInstance;
}

static inline WebView* toCpp(Evas_Object* o)
{
    View_Smart_Data* sd = (View_Smart_Data*)evas_object_smart_data_get(o);
    if (!sd)
        return 0;

    return sd->cppInstance;
}

static Evas_Object* ewkViewAdd(Evas_Object* parent, WebView* webView);

void WebView::initialize()
{
    ewk_init();

    const char* httpProxy = getenv("http_proxy");
    if (httpProxy)
        ewk_network_proxy_uri_set(httpProxy);
}

WebView::WebView(Object* container)
{
    Evas* evas = evas_object_evas_get(container->object());
    //Evas_Object* ewkView = ewk_view_single_add(evas);
    Evas_Object* ewkView = ewkViewAdd(container->object(), this);

    setObject(ewkView);
}

static void ewkViewSmartAdd(Evas_Object* o)
{
    View_Smart_Data* sd = (View_Smart_Data*)calloc(1, sizeof(View_Smart_Data));
    evas_object_smart_data_set(o, sd);

    _parent_sc.sc.add(o);
}

void ewkViewSmartDel(Evas_Object* o)
{
    _parent_sc.sc.del(o);
}

static Evas_Object* ewkViewSmartWindowCreate(Ewk_View_Smart_Data *sd, Eina_Bool javascript, const Ewk_Window_Features *window_features)
{
    //FIXME
    return 0;
}

void ewkViewInitialize(Evas_Object* o, WebView* webView)
{
    View_Smart_Data* sd = (View_Smart_Data*)evas_object_smart_data_get(o);
    if (!sd)
        return;

    sd->cppInstance = webView;
}

Evas_Object* ewkViewAdd(Evas_Object* parent, WebView* webView)
{
    static Evas_Smart* smart = 0;
    Evas* canvas = evas_object_evas_get(parent);

    if (!smart) {
        static Ewk_View_Smart_Class api = EWK_VIEW_SMART_CLASS_INIT_NAME_VERSION("EWK_View_Demo");
        ewk_view_single_smart_set(&api);
        ewk_view_single_smart_set(&_parent_sc);

        api.sc.add = ewkViewSmartAdd;
        api.sc.del = ewkViewSmartDel;

        api.window_create = ewkViewSmartWindowCreate;

        smart = evas_smart_class_new(&api.sc);
        if (!smart)
            return 0;
    }
    Evas_Object* ewkView = evas_object_smart_add(canvas, smart);
    ewkViewInitialize(ewkView, webView);
    return ewkView;
}
