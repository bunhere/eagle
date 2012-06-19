/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <Elementary.h>
#include <browser/WebView.h>

#if USE_WEBKIT
#include <EWebKit.h>
#else
#include <EWebKit2.h>
#endif

void WebView::onMouseDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    evas_object_focus_set(ewkObject, true);
}

WebView::~WebView()
{
    evas_object_event_callback_del(object(), EVAS_CALLBACK_MOUSE_DOWN, onMouseDown);
}

WebView* WebView::create(Object* container)
{
    WebView* webview = new WebView(container);

    evas_object_size_hint_weight_set(webview->object(), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    evas_object_event_callback_add(webview->object(), EVAS_CALLBACK_MOUSE_DOWN, onMouseDown, webview);
    return webview;
}

void WebView::loadUrl(const char* url)
{
    ewk_view_uri_set(object(), url);
}
