/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser.h"

#include "Urlbar.h"
#include "MultitabBar.h"
#include "Tab.h"
#include "WebView.h"
#include <Elementary.h>
#include <algorithm>

BrowserConfig::BrowserConfig()
    : urlbar(true)
    , multitapBar(true)
{
}

BrowserContent::~BrowserContent()
{
    if (m_title)
        free(m_title);
}

void BrowserContent::setTitle(const char* title)
{
    if (!title || !title[0])
        return;

    m_title = strdup(title);

    if (m_tab) {
        m_tab->setTitle(m_title);
    }
}

void BrowserContent::createTabIfNeeded()
{
    if (m_tab)
        return;

    m_tab = new Tab(this);
}

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
{
    m_layout = elm_layout_add(object());
    //FIXME: add error handling

    if (!elm_layout_file_set(m_layout, themePath(), "eagle/browser-view")) {
        //FIXME: add error handling
        printf("%s theme path is failed\n", themePath());
        return;
    }

    evas_object_size_hint_weight_set(m_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(object(), m_layout);
    evas_object_show(m_layout);

    chooseContent(WebView::create(this));

    if (config.urlbar) {
        m_urlbar = new Urlbar(this);
        elm_object_part_content_set(m_layout, "sw.urlbar", m_urlbar->object());
    } else {
        m_urlbar = 0;
        edje_object_signal_emit(elm_layout_edje_get(m_layout), "disable_urlbar", "");
    }

    if (config.multitapBar) {
        m_multitapBar = new MultitabBar(this);
        updateMultitab();
        elm_object_part_content_set(m_layout, "sw.multitabbar", m_multitapBar->object());
    } else {
        m_multitapBar = 0;
        edje_object_signal_emit(elm_layout_edje_get(m_layout), "disable_multitabbar", "");
    }

    elm_object_focus_set(m_layout, true);
}

Browser::~Browser()
{
    size_t size = m_contents.size();
    for (size_t i = 0; i < size; ++i)
        delete m_contents[i];

    if (m_urlbar)
        delete m_urlbar;

    if (m_multitapBar)
        delete m_multitapBar;
}

void Browser::loadUrl(const char* url)
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    webView->loadUrl(url);
}

void Browser::back()
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    webView->back();
}

void Browser::forward()
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    webView->forward();
}

void Browser::reload()
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    webView->reload();
}

void Browser::stop()
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    webView->stop();
}

void Browser::setInspector(const WebView* inspector)
{
    if (!inspector)
        elm_object_part_content_unset(m_layout, "sw.inspector");
    else
        elm_object_part_content_set(m_layout, "sw.inspector", inspector->object());
}

const char* Browser::themePath()
{
    //FIXME: Need to find real path
    return THEME_DIR "/eagle.edj";
}

void Browser::resize(int width, int height)
{
    Object::resize(width, height);

    m_content->resize(width, height);
}

void Browser::executeShortCut(const char* key, bool ctrlPressed, bool altPressed)
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    if (ctrlPressed) {
        if (!strcmp(key, "i"))
            webView->openInspectorView();
        else if (!strcmp(key, "KP_Add"))
            webView->scaleUp();
        else if (!strcmp(key, "KP_Subtract"))
            webView->scaleDown();
    } else if (altPressed) {
        if (!strcmp(key, "Left"))
            webView->back();
        else if (!strcmp(key, "Right"))
            webView->forward();
    }
}

void Browser::attachContent(BrowserContent* content)
{
    m_contents.push_back(content);

    if (m_multitapBar)
        m_multitapBar->update();
}

void Browser::detachContent(BrowserContent* content)
{
    m_contents.erase(std::find(m_contents.begin(), m_contents.end(), content));
}

void Browser::chooseContent(BrowserContent* content)
{
    if (m_content == content)
        return;

    if (content)
        content->hide();

    m_content = content;
    elm_object_part_content_set(m_layout, "sw.bcontent", m_content->object());
    m_content->show();
}

void Browser::updateMultitab()
{
    if (!m_multitapBar)
        return;

    m_multitapBar->update();
}
