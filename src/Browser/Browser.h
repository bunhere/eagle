/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Browser_h
#define Browser_h

#include "EflWrappers/Window.h"

class Urlbar;
class WebView;

class BrowserConfig
{
public:
    bool urlbar;
};

class Browser : public Window
{
public:
    static void initialize();
    static Browser* create(const BrowserConfig& config);
    static Browser* create();

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();

    void createInspector(WebView* receivedWebView);
    void closeInspector();
    void executeShortCut(const char* key, bool ctrlPressed, bool altPressed);
    const char* getTheme();

    virtual void resize(int width, int height);

    Urlbar* urlbar() { return m_urlbar; }
private:
    explicit Browser(const BrowserConfig&);

    Urlbar* m_urlbar;

    Evas_Object* m_layout;
    WebView* m_inspector;
    WebView* m_webView;
};

#endif
