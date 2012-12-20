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

#define SMART_CALLBACK_DECLARE(func) \
    static void func(void *user_data, Evas_Object *webview, void *event_info)

#define SIGNAL_FUNC_DECLARE(function) \
    static void function(void* data, Evas_Object* obj, const char* emission, const char* source)

class Object
{
public:
    explicit Object(Evas_Object* o) : m_object(o) {}
    virtual ~Object() {
        if (!m_object)
            evas_object_del(m_object);
    }

    void show();
    void hide();
    void move(int x, int y);
    virtual void resize(int width, int height);

    int width() { return m_width; }
    int height() { return m_height; }

    Evas_Object* object() const { return m_object; }
private:
    Evas_Object* m_object;

    int m_width, m_height;
};
#endif
