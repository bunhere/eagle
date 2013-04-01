/*
 * Copyright (C) 2013 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser/Browser.h"
#include "Browser/WebView.h"
#include <EWebKit.h>

Evas_Object* ewkViewAdd(Evas_Object* parent, WebView* webView)
{
    return elm_web_add(parent);
}
