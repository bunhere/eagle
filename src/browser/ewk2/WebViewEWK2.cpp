/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <EWebKit2.h>
#include <Elementary.h>
#include <browser/Browser.h>
#include <browser/WebView.h>

WebView::WebView(Browser* container)
    : m_container(container)
{
    Evas* evas = evas_object_evas_get(container->object());
    Evas_Object* ewkView = ewk_view_add(evas);

    setObject(ewkView);
}
