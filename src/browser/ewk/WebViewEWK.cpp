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
    Evas_Object* ewkView = ewk_view_single_add(evas);
    evas_object_size_hint_weight_set(ewkView, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(container->object(), ewkView);

    setObject(ewkView);
}
