/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Object_h
#define Object_h

#include <Evas.h>

class Object
{
public:
    void show();
    void move(int x, int y);
    virtual void resize(int width, int height);

    int width() { return m_width; }
    int height() { return m_height; }

    Evas_Object* object() { return m_object; }
    void setObject(Evas_Object* obj) { m_object = obj; }

private:
    Evas_Object* m_object;

    int m_width, m_height;
};
#endif