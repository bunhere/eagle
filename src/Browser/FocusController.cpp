/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "FocusController.h"

#include "Browser.h"
#include "EflWrappers/Object.h"
#include "WebView.h"
#include <Elementary.h>

static Object* previousObject;
static bool isWebViewFocused;

void FocusController::setFocus(Object* object)
{
    fprintf(stderr, "%s (%d)\n", __func__, isWebViewFocused);
    if (isWebViewFocused)
        evas_object_focus_set(previousObject->object(), false);

    elm_object_focus_set(object->object(), true);
    isWebViewFocused = false;
}

void FocusController::setFocus(WebView* webview)
{
    fprintf(stderr, "%s 2 (%d)\n", __func__, isWebViewFocused);
    if (isWebViewFocused == false)
        elm_object_focus_set(elm_object_top_widget_get(webview->container()->object()), false);

    evas_object_focus_set(webview->object(), true);

    previousObject = webview;
    isWebViewFocused = true;
}

void FocusController::lostFocus(WebView* webview)
{
    isWebViewFocused = false;
}
