/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser/Browser.h"
#include "Browser/WebView.h"
#include <EWebKit2.h>
#include <Elementary.h>

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

Evas_Object* ewkViewAdd(Evas_Object* parent, WebView* webView)
{
    static Evas_Smart* smart = 0;
    Evas* canvas = evas_object_evas_get(parent);

    if (!smart) {
        static Ewk_View_Smart_Class api = EWK_VIEW_SMART_CLASS_INIT_NAME_VERSION("EWK_View_Demo");
        ewk_view_smart_class_set(&api);
        ewk_view_smart_class_set(&_parent_sc);

        api.sc.add = ewkViewSmartAdd;
        api.sc.del = ewkViewSmartDel;

        smart = evas_smart_class_new(&api.sc);
        if (!smart)
            return 0;
    }

    Evas_Object* ewkView = ewk_view_smart_add(canvas, smart, ewk_context_default_get());
    View_Smart_Data* sd = (View_Smart_Data*)evas_object_smart_data_get(ewkView);
    sd->cppInstance = webView;

    return ewkView;
}
