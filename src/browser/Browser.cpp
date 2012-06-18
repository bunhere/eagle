/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser.h"

#include <Elementary.h>
#include <browser/WebView.h>

bool Browser::s_initialized = false;

Browser* Browser::create()
{
    if (!s_initialized) {
        WebView::initialize();

        s_initialized = true;
    }

    Browser* newBrowser = new Browser;
    if (!newBrowser->object()) {
        delete newBrowser;
        fprintf(stderr, "Could not create Window.\n");
        return 0;
    }
    return newBrowser;
}

static void urlbarChanged(void* data, Evas_Object* obj, void* eventInfo)
{
    printf(" [%s]\n", elm_object_text_get(obj));
}

static void urlbarActivated(void* data, Evas_Object* obj, void* eventInfo)
{
    Browser* browser = static_cast<Browser*>(data);
    const char* url = elm_object_text_get(obj);
    printf(" [%s entered]\n", elm_object_text_get(obj));
    browser->loadUrl(elm_entry_markup_to_utf8(url));
}

Browser::Browser()
{
    m_layout = elm_layout_add(object());
    //FIXME: add error handling

    if (!elm_layout_file_set(m_layout, getTheme(), "eagle/browser-view")) {
        //FIXME: add error handling
        printf("%s theme path is failed\n", getTheme());
        return;
    }
	evas_object_size_hint_weight_set(m_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(m_layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(object(), m_layout);
	evas_object_show(m_layout);

    m_entry = elm_entry_add(m_layout);
    elm_entry_single_line_set(m_entry, true);

    evas_object_size_hint_weight_set(m_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);

    //evas_object_smart_callback_add(m_entry, "changed", urlbarChanged, this);
    evas_object_smart_callback_add(m_entry, "activated", urlbarActivated, this);

    elm_object_part_content_set(m_layout, "sw.urlbar", m_entry);

    m_webView = new WebView(this);
    elm_object_part_content_set(m_layout, "sw.webview", m_webView->object());
    m_webView->show();
}

void Browser::loadUrl(const char* url)
{
    elm_object_text_set(m_entry, url);
    printf(" %s will be loaded\n", url);
    m_webView->loadUrl(url);
}

const char* Browser::getTheme()
{
    //FIXME: Need to find real path
    return "./theme/eagle.edj";
}

void Browser::resize(int width, int height)
{
    Object::resize(width, height);

    m_webView->resize(width, height);
}
