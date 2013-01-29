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

void Browser::initialize()
{
    WebView::initialize();

    // register shortcuts.
    ShortCut& s = ShortCut::instance();
    s.addCommand('i', ShortCut::CTRL, openInspectorView);
    s.addCommand('t', ShortCut::CTRL, addNewPage);
    s.addCommand('w', ShortCut::CTRL, closePage);

    s.addCommand("KP_Add", ShortCut::CTRL, scaleUp);
    s.addCommand("KP_Subtract", ShortCut::CTRL, scaleDown);

    s.addCommand("Left", ShortCut::ALT, back);
    s.addCommand("Right", ShortCut::ALT, forward);
}

BrowserConfig::BrowserConfig()
    : urlbar(true)
    , multitapBar(true)
{
}

BrowserContent::~BrowserContent()
{
    if (m_title)
        free(m_title);

    if (m_tab)
        delete m_tab;
}

void BrowserContent::setTitle(const char* title)
{
    if (!title || !title[0])
        return;

    m_title = strdup(title);

    if (m_tab) {
        m_tab->setTitle(m_title);
    }

    m_container->titleChanged(this);
}

void BrowserContent::createTabIfNeeded()
{
    if (m_tab)
        return;

    m_tab = new Tab(this);

    if (m_container->isActiveContent(this))
        m_tab->setActive(true);
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

void Browser::onFocusIn(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    fprintf(stderr, "Browser %s called\n", __func__);
    Browser* browser = static_cast<Browser*>(data);
    browser->m_content->setFocus(true);
}

void Browser::onFocusOut(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    fprintf(stderr, "Browser %s called\n", __func__);
}

Browser::Browser(const BrowserConfig& config)
{
    evas_object_event_callback_add(object(), EVAS_CALLBACK_FOCUS_IN, onFocusIn, this);
    evas_object_event_callback_add(object(), EVAS_CALLBACK_FOCUS_OUT, onFocusOut, this);

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

    addNewPage(0);

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

#define COMMAND_WEBVIEW_IMPLEMENT(name) \
bool Browser::name(const ShortCut::CommandInfo*, Browser* browser, BrowserContent* content) \
{ \
    if (!content) \
        return name(0, browser, browser->m_content); \
 \
    if (!content->isWebView()) \
        return false; \
 \
    WebView* webView = (WebView*) content; \
    webView->name(); \
 \
    return true; \
}

#define COMMAND_BROWSER_IMPLEMENT(name) \
bool Browser::name(const ShortCut::CommandInfo*, Browser* browser, BrowserContent* content) \
{ \
    browser->name(content); \
    return true; \
}

COMMAND_WEBVIEW_IMPLEMENT(openInspectorView)
COMMAND_WEBVIEW_IMPLEMENT(back)
COMMAND_WEBVIEW_IMPLEMENT(forward)
COMMAND_WEBVIEW_IMPLEMENT(scaleUp)
COMMAND_WEBVIEW_IMPLEMENT(scaleDown)

COMMAND_BROWSER_IMPLEMENT(addNewPage)
COMMAND_BROWSER_IMPLEMENT(closePage)

void Browser::loadUrl(const char* url)
{
    if (!m_content->isWebView())
        return;

    WebView* webView = (WebView*) m_content;
    webView->loadUrl(url);
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

void Browser::titleChanged(BrowserContent* content)
{
    if (m_content != content)
        return;

    if (m_content->title())
        setTitle(m_content->title());
    else
        setTitle(PROJECT_NAME);
}

void Browser::urlChanged(BrowserContent* content)
{
    if (m_content != content)
        return;

    if (m_urlbar)
        m_urlbar->changeUrlEntry(content->url());
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

void Browser::addNewPage(BrowserContent*)
{
    attachContent(WebView::create(this), true);
}

void Browser::closePage(BrowserContent* content)
{
    if (!content)
        content = m_content;

    detachContent(content);
    delete content;
}

void Browser::attachContent(BrowserContent* content, bool active)
{
    m_contents.push_back(content);

    if (active)
        chooseContent(content);
}

void Browser::detachContent(BrowserContent* content)
{
    if (m_content == content) {
        // FIXME : Need better way to find previous content
        size_t size = m_contents.size();
        if (size == 1) {
            evas_object_smart_callback_call(object(), "delete,request", this);
            return;
        }

        size_t newIndex = 0;
        for (size_t i = 0; i < size; ++i)
            if (m_content == m_contents[i]) {
                if (i == 0)
                    newIndex = 1;
                else
                    newIndex = i - 1;
                break;
            }

        chooseContent(m_contents[newIndex], false);
    }
    m_contents.erase(std::find(m_contents.begin(), m_contents.end(), content));

    updateMultitab();
}

void Browser::chooseContent(BrowserContent* content, bool update)
{
    if (m_content == content)
        return;

    if (m_content) {
        if (m_content->tab())
            m_content->tab()->setActive(false);

        m_content->setFocus(false);
        m_content->hide();
    }

    m_content = content;
    elm_object_part_content_unset(m_layout, "sw.bcontent"); // crash point?
    elm_object_part_content_set(m_layout, "sw.bcontent", m_content->object());

    if (m_content->tab())
        m_content->tab()->setActive(true);

    urlChanged(m_content);
    titleChanged(m_content);
    m_content->setFocus(true);

    m_content->show();

    if (update)
        updateMultitab();
}

void Browser::updateMultitab()
{
    if (!m_multitapBar)
        return;

    m_multitapBar->update();
}
