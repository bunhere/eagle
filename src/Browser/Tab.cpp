/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Tab.h"

#include "WebView.h"
#include <Elementary.h>

Tab::Tab(BrowserContent* container)
    : Object(elm_layout_add(container->object()))
    , m_active(false)
{
    if (!elm_layout_file_set(object(), Browser::themePath(), "eagle/multitab")) {
        return;
    }
    evas_object_size_hint_weight_set(object(), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(object(), EVAS_HINT_FILL, EVAS_HINT_FILL);

    m_title = elm_label_add(container->object());
    printf("TTEST\n");
    elm_object_text_set(m_title, "TEST");
    elm_object_part_content_set(object(), "title", m_title);

    evas_object_show(m_title);
}

void Tab::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;

    Edje_Message_Int_Set msg;// = malloc(sizeof(Edje_Message_Int_Set) + sizeof(int));
    msg.count = 1;
    msg.val[0] = (int) active;

    edje_object_message_send(elm_layout_edje_get(object()), EDJE_MESSAGE_INT_SET, 0, &msg);
}

void Tab::setTitle(const char* title)
{
    elm_object_text_set(m_title, title);
}
