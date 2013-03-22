/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser/WebView.h"

#include "Browser/Browser.h"
#include "Browser/PopupMenu.h"
#include <EWebKit2.h>
#include <Elementary.h>

static Ewk_View_Smart_Class _parent_sc = EWK_VIEW_SMART_CLASS_INIT_NULL;
typedef struct _View_Smart_Data View_Smart_Data;
struct _View_Smart_Data
{
    Ewk_View_Smart_Data base;
    WebView* cppInstance;
    PopupMenu* popup;
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
    View_Smart_Data* vsd = (View_Smart_Data*)evas_object_smart_data_get(o);
    if (vsd && vsd->popup)
        delete vsd->popup;

    _parent_sc.sc.del(o);
}

static Eina_Bool showPopupMenu(Ewk_View_Smart_Data* sd, Eina_Rectangle rect, Ewk_Text_Direction textDirection, double pageScaleFactor, Ewk_Popup_Menu* popupMenu)
{
    View_Smart_Data* vsd = (View_Smart_Data*)sd;
    if (vsd->popup)
        delete vsd->popup;

    vsd->popup = PopupMenu::create(sd->self, rect, popupMenu);

    return true;
}

static Eina_Bool hidePopupMenu(Ewk_View_Smart_Data *sd)
{
    View_Smart_Data* vsd = (View_Smart_Data*)sd;
    vsd->popup->hide();
    delete vsd->popup;
    vsd->popup = 0;

    return true;
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

        api.popup_menu_show = showPopupMenu;
        api.popup_menu_hide = hidePopupMenu;

        smart = evas_smart_class_new(&api.sc);
        if (!smart)
            return 0;
    }

    Evas_Object* ewkView = ewk_view_smart_add(canvas, smart, ewk_context_default_get(), ewk_page_group_create(""));
    View_Smart_Data* sd = (View_Smart_Data*)evas_object_smart_data_get(ewkView);
    sd->cppInstance = webView;
    sd->popup = 0;

    return ewkView;
}
