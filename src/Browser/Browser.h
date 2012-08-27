/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Browser_h
#define Browser_h

#include "Browser/Urlbar.h"
#include "EflWrappers/Window.h"

class Urlbar;
class WebView;

class Browser : public Window
{
public:
    static Browser* create();

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();

    void createInspector(WebView* receivedWebView);
    void closeInspector();
    const char* getTheme();

    virtual void resize(int width, int height);

    Urlbar& urlbar() { return m_urlbar; }
private:
    Browser();

    static bool s_initialized;
    Urlbar m_urlbar;

    Evas_Object* m_layout;
    WebView* m_inspector;
    WebView* m_webView;
};

#endif
