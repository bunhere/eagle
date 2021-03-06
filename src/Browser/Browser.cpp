/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser.h"

#include "BrowserContent.h"
#include "ERU/Logger.h"
#include "MultitabBar.h"
#include "Tab.h"
#include "Urlbar.h"
#include "WebView.h"
#include <Elementary.h>
#include <algorithm>

#ifdef USE_MOBILE_PROFILE
static const int sDefaultWidth = 480;
static const int sDefaultHeight = 800;
#else
static const int sDefaultWidth = 0;
static const int sDefaultHeight = 0;
#endif

void Browser::initialize()
{
    WebView::initialize();

    // register shortcuts.
    ShortCut& s = ShortCut::instance();
    s.addCommand('i', ShortCut::CTRL, openInspectorView);
    s.addCommand('n', ShortCut::CTRL, createNewBrowser);
    s.addCommand('t', ShortCut::CTRL, addNewPage);
    s.addCommand('u', ShortCut::CTRL, setSourceMode);
    s.addCommand('w', ShortCut::CTRL, closePage);

    s.addCommand("KP_Add", ShortCut::CTRL, scaleUp);
    s.addCommand("KP_Subtract", ShortCut::CTRL, scaleDown);

    s.addCommand('d', ShortCut::ALT, focusAndSelectUrlBar);
    s.addCommand("Left", ShortCut::ALT, back);
    s.addCommand("Right", ShortCut::ALT, forward);

    for (int i = 1; i < 10; ++i)
        s.addCommand('0' + i, ShortCut::ALT, movePage);

    s.addCommand("F11", ShortCut::NONE, toggleFullScreen);
}

BrowserConfig::BrowserConfig()
    : width(sDefaultWidth)
    , height(sDefaultHeight)
    , urlbar(true)
    , multitapBar(true)
{
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
    LOG("");
    Browser* browser = static_cast<Browser*>(data);
    browser->m_content->setFocus(true);
}

void Browser::onFocusOut(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    LOG("");
}

Browser::Browser(const BrowserConfig& config)
    : Window(config.width, config.height)
    , m_content(0)
    , m_urlbar(0)
    , m_multitapBar(0)
    , m_isEnteredFullScreen(false)
{
    evas_object_event_callback_add(object(), EVAS_CALLBACK_FOCUS_IN, onFocusIn, this);
    evas_object_event_callback_add(object(), EVAS_CALLBACK_FOCUS_OUT, onFocusOut, this);

    m_layout = elm_layout_add(object());
    //FIXME: add error handling

    if (!elm_layout_file_set(m_layout, themePath(), "eagle/browser-view")) {
        //FIXME: add error handling
        fprintf(stderr, "%s theme path is failed\n", themePath());
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

COMMAND_WEBVIEW_IMPLEMENT(back)
COMMAND_WEBVIEW_IMPLEMENT(forward)
COMMAND_WEBVIEW_IMPLEMENT(openInspectorView)
COMMAND_WEBVIEW_IMPLEMENT(scaleUp)
COMMAND_WEBVIEW_IMPLEMENT(scaleDown)
COMMAND_WEBVIEW_IMPLEMENT(setSourceMode)

COMMAND_BROWSER_IMPLEMENT(createNewBrowser)
COMMAND_BROWSER_IMPLEMENT(addNewPage)
COMMAND_BROWSER_IMPLEMENT(closePage)
COMMAND_BROWSER_IMPLEMENT(focusAndSelectUrlBar)
COMMAND_BROWSER_IMPLEMENT(toggleFullScreen)

bool Browser::movePage(const ShortCut::CommandInfo* command, Browser* browser, BrowserContent*)
{
    int index = command->key()[0] - '0' - 1;
    if (index >= browser->m_contents.size())
        return false;

    browser->chooseContent(browser->m_contents[index]);
    return true;
}

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

void Browser::backFordwardListChanged(BrowserContent* content, bool backPossible, bool forwardPossible)
{
    if (m_content != content)
        return;

    if (m_urlbar)
        m_urlbar->updateBackFordwardButton(backPossible, forwardPossible);
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

void Browser::createNewBrowser(BrowserContent*)
{
    Browser* defaultWindow = Browser::create();
    if (!defaultWindow)
        return;

    defaultWindow->setTitle("Eagle");
    defaultWindow->show();
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
    m_contents.erase(std::find(m_contents.begin(), m_contents.end(), content));
    delete content;

    updateMultitab();
}

void Browser::focusAndSelectUrlBar(BrowserContent*)
{
    m_urlbar->focusAndSelectAll();
}

void Browser::toggleFullScreen(BrowserContent*)
{
    m_isEnteredFullScreen = !m_isEnteredFullScreen;
    elm_win_fullscreen_set(object(), m_isEnteredFullScreen);
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
