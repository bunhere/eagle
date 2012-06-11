/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Window_h
#define Window_h

#include "EflWrappers/Object.h"

class Window : public Object
{
public:
    Window();

    void setTitle(const char* title);

    /* smart callback functions */
    static void onDeleteRequest(void* data, Evas_Object* obj, void* eventInfo);

private:
    Evas_Object* m_bg;

    static Window* head;
    Window* next;
};

#endif
