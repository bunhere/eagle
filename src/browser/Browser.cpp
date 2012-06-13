/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser.h"

#include <browser/WebView.h>

Browser* Browser::create()
{
    Browser* newBrowser = new Browser;
    if (!newBrowser->object()) {
        delete newBrowser;
        fprintf(stderr, "Could not create Window.\n");
        return 0;
    }
    return newBrowser;
}

Browser::Browser()
{
    m_webView = new WebView(this);
    m_webView->show();
    m_webView->move(0, 0);
}

void Browser::loadUrl(const char* url)
{
    m_webView->loadUrl(url);
}

void Browser::resize(int width, int height)
{
    Object::resize(width, height);

    m_webView->resize(width, height);
}
