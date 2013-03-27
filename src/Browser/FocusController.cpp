/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "FocusController.h"

#include "Browser.h"
#include "ERU/Logger.h"
#include "EflWrappers/Object.h"
#include "WebView.h"
#include <Elementary.h>

static Object* previousObject = 0;
static bool isWebViewFocused;

void FocusController::setFocus(Object* object)
{
    LOG("Object (%p == %p), isWebViewFocused:%d", object, previousObject, isWebViewFocused);
    if (object == previousObject)
        return;

    LOG("isWebViewFocused : %d", isWebViewFocused);
    if (isWebViewFocused)
        evas_object_focus_set(previousObject->object(), false);

    elm_object_focus_set(object->object(), true);
    previousObject = object;
    isWebViewFocused = false;
}

void FocusController::setFocus(WebView* webview)
{
    LOG("WebView (%p == %p), isWebViewFocused:%d", webview, previousObject, isWebViewFocused);
    if (webview == previousObject)
        return;

    if (isWebViewFocused == false) {
        if (previousObject)
            elm_object_focus_set(previousObject->object(), false);
        elm_object_focus_set(elm_object_top_widget_get(webview->container()->object()), false);
    }

    evas_object_focus_set(webview->object(), true);
    previousObject = webview;
    isWebViewFocused = true;
}

void FocusController::lostFocus(WebView* webview)
{
    if (webview != previousObject)
        return;

    previousObject = 0;
    isWebViewFocused = false;
}
