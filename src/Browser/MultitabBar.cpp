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

    m_addButton = elm_button_add(container->object());
    elm_object_text_set(m_addButton, "+");
    evas_object_smart_callback_add(m_addButton, "clicked", add_clicked, this);
    evas_object_show(m_addButton);

    evas_object_size_hint_min_set(m_addButton, URLBAR_BUTTON_SIZE, URLBAR_BUTTON_SIZE);
}

MultitabBar::~MultitabBar()
{
    evas_object_del(m_addButton);
}

void MultitabBar::add_clicked(void* data, Evas_Object*, void*)
{
    MultitabBar* self = static_cast<MultitabBar*>(data);
    self->container()->addPage();
}

void MultitabBar::update()
{
    elm_box_unpack_all(object());

    size_t size = m_container->contentsSize();

    for (size_t i = 0; i < size; ++i) {
        BrowserContent* content = m_container->contentsAt(i);
        content->createTabIfNeeded();
        elm_box_pack_end(object(), content->tab()->object());
        evas_object_show(content->tab()->object());
    }
    elm_box_pack_end(object(), m_addButton);
}
