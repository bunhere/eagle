/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Window.h"

#include <Elementary.h>
#include <stdio.h>

Window* Window::head = 0;

Window::Window(int width, int height)
    : Object(elm_win_add(0, PROJECT_NAME, ELM_WIN_BASIC))
{
    if (!object())
        return;

    if (!head)
        next = head;
    else
        next = 0;
    head = this;

    evas_object_smart_callback_add(object(), "delete,request", onDeleteRequest, this);
    m_bg = elm_bg_add(object());

    evas_object_size_hint_weight_set(m_bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(object(), m_bg);
    evas_object_show(m_bg);

    fprintf(stderr, "%d, %d\n", width, height);
    resize(width, height);
}

void Window::setTitle(const char* title)
{
    elm_win_title_set(object(), title);
}

void Window::onDeleteRequest(void* data, Evas_Object* obj, void* eventInfo)
{
    //FIXME
    elm_exit();
}
