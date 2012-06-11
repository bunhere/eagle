/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "WebView.h"

#include <EWebKit2.h>
#include <Elementary.h>

WebView::WebView(Object* container)
{
    Evas* evas = evas_object_evas_get(container->object());
    Evas_Object* ewkView = ewk_view_add(evas);
    evas_object_size_hint_weight_set(ewkView, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(container->object(), ewkView);

    setObject(ewkView);
}

void WebView::loadUrl(const char* url)
{
    ewk_view_uri_set(object(), url);
}
