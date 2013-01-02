/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "config.h"
#include "MultitabBar.h"

#include "Browser.h"
#include "Tab.h"
#include "WebView.h"
#include <Edje.h>
#include <Elementary.h>

MultitabBar::MultitabBar(Browser* container)
    : Object(elm_box_add(container->object()))
    , m_container(container)
{
    elm_box_horizontal_set(object(), true);
    evas_object_size_hint_weight_set(object(), EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(object(), EVAS_HINT_FILL, 0.0);
}

void MultitabBar::update()
{
    elm_box_unpack_all(object());

    size_t size = m_container->contentsSize();
    fprintf(stderr, " [ %d]\n", size);

    for (size_t i = 0; i < size; ++i) {
        BrowserContent* content = m_container->contentsAt(i);
        content->createTabIfNeeded();
        elm_box_pack_end(object(), content->tab()->object());
        evas_object_show(content->tab()->object());
    }
}
