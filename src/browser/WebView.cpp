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

void WebView::onKeyDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down*) event_info;
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(evas_key_modifier_get(e), "Control");

#if USE_WEBKIT
#else
    if (ctrlPressed) {
        if (!strcmp(ev->key, "KP_Add")) {
            double ratio = ewk_view_device_pixel_ratio_get(ewkObject);
            ewk_view_device_pixel_ratio_set(ewkObject, ratio * 1.1);
        } else if (!strcmp(ev->key, "KP_Subtract")) {
            double ratio = ewk_view_device_pixel_ratio_get(ewkObject);
            ewk_view_device_pixel_ratio_set(ewkObject, ratio * 0.9);
        }
    }
#endif
}

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

    //FIXME: hard typed path is bad.
    ewk_view_theme_set(webview->object(), "/usr/local/share/ewebkit-0/themes/default.edj");

    evas_object_size_hint_weight_set(webview->object(), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    evas_object_event_callback_add(webview->object(), EVAS_CALLBACK_KEY_DOWN, onKeyDown, webview);
    evas_object_event_callback_add(webview->object(), EVAS_CALLBACK_MOUSE_DOWN, onMouseDown, webview);
    return webview;
}

void WebView::loadUrl(const char* url)
{
    ewk_view_uri_set(object(), url);
}

void WebView::back()
{
    ewk_view_back(object());
}

void WebView::forward()
{
    ewk_view_forward(object());
}

void WebView::reload()
{
    ewk_view_reload(object());
}

void WebView::stop()
{
    ewk_view_stop(object());
}
