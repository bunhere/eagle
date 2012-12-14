/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser.h"

#include "Urlbar.h"
#include "WebView.h"
#include <Elementary.h>

void Browser::initialize()
{
    WebView::initialize();
}

Browser* Browser::create()
{
    BrowserConfig config;
    return create(config);
}

Browser* Browser::create(const BrowserConfig& config)
{
    Browser* newBrowser = new Browser(config);
    if (!newBrowser->object()) {
        delete newBrowser;
        fprintf(stderr, "Could not create Window.\n");
        return 0;
    }
    return newBrowser;
}

Browser::Browser(const BrowserConfig& config)
    : m_inspector(0)
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

    if (config.urlbar) {
        m_urlbar = new Urlbar(this);
        elm_object_part_content_set(m_layout, "sw.urlbar", m_urlbar->object());
    } else
        m_urlbar = 0;

    m_webView = WebView::create(this);
    elm_object_part_content_set(m_layout, "sw.webview", m_webView->object());
    m_webView->show();

    elm_object_focus_set(m_layout, true);
}

void Browser::loadUrl(const char* url)
{
    m_webView->loadUrl(url);
}

void Browser::back()
{
    m_webView->back();
}

void Browser::forward()
{
    m_webView->forward();
}

void Browser::reload()
{
    m_webView->reload();
}

void Browser::stop()
{
    m_webView->stop();
}

void Browser::createInspector(WebView* receivedWebView)
{
    printf( "%s is called\n", __func__);
    if (!m_inspector) {
        m_inspector = WebView::create(this);
        elm_object_part_content_set(m_layout, "sw.inspector", m_inspector->object());
        m_inspector->show();
    }
    receivedWebView->setInspectorView(*m_inspector);
}

void Browser::closeInspector()
{
    printf( "%s is called\n", __func__);
    if (!m_inspector)
        return;

    delete m_inspector;
    m_inspector = 0;
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

void Browser::executeShortCut(const char* key, bool ctrlPressed, bool altPressed)
{
    if (ctrlPressed) {
        if (!strcmp(key, "i"))
            m_webView->openInspectorView();
        else if (!strcmp(key, "KP_Add"))
            m_webView->scaleUp();
        else if (!strcmp(key, "KP_Subtract"))
            m_webView->scaleDown();
    }
}
