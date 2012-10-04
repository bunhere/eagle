/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef PopupMenu_h
#define PopupMenu_h

#include "EflWrappers/Object.h"

class PopupMenu : public Object
{
public:
    static PopupMenu& instance();

    void create(Evas_Object* ewkView, Eina_Rectangle rect, Eina_List* items, int selectedIndex);
    void destroy();
private:
    PopupMenu();

    static void menuItemSelected(void*, Evas_Object*, void*);

    Evas_Object* m_currentView;
};
#endif