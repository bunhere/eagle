/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Object.h"

#include <Evas.h>

void Object::show()
{
    evas_object_show(m_object);
}

void Object::move(int x, int y)
{
    evas_object_move(m_object, x, y);
}

void Object::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    evas_object_resize(m_object, width, height);
}
