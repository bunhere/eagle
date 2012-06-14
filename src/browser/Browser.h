/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Browser_h
#define Browser_h

#include <EflWrappers/Window.h>

class WebView;

class Browser : public Window
{
public:
    static Browser* create();

    void loadUrl(const char* url);

    virtual void resize(int width, int height);
private:
    Browser();

    static bool s_initialized;
    WebView* m_webView;
};

#endif
