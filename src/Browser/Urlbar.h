/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Urlbar_h
#define Urlbar_h

#include <EflWrappers/Object.h>

class Browser;

class Urlbar : public Object
{
public:
    Urlbar(Browser* container);
    ~Urlbar() {}

    Browser* container() { return m_container; };
    void changeUrlEntry(const char* url);
    //static void onMouseDown(void* data, Evas* e, Evas_Object* webview, void* event_info);

    static void onKeyDown(void*, Evas*, Evas_Object*, void*);

    SIGNAL_FUNC_DECLARE(back_clicked);
    SIGNAL_FUNC_DECLARE(forward_clicked);
    SIGNAL_FUNC_DECLARE(reload_clicked);
    SIGNAL_FUNC_DECLARE(stop_clicked);
private:
    Browser* m_container;
    Evas_Object* m_entry;
};

#endif
