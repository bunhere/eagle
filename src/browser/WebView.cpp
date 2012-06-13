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

void WebView::loadUrl(const char* url)
{
    ewk_view_uri_set(object(), url);
}

