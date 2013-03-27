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
    explicit Urlbar(Browser* container);
    ~Urlbar() {}

    Browser* container() { return m_container; };
    void changeUrlEntry(const char* url);
    void updateBackFordwardButton(bool backPossible, bool forwardPossible);
    void focusAndSelectAll();

    static void filterPrepend(void* data, Evas_Object* entry, char** text);

    static void onKeyDown(void*, Evas*, Evas_Object*, void*);
    static void onMouseDown(void*, Evas*, Evas_Object*, void*);

    SMART_CALLBACK_DECLARE(back_clicked);
    SMART_CALLBACK_DECLARE(forward_clicked);
    SMART_CALLBACK_DECLARE(reload_clicked);
    SMART_CALLBACK_DECLARE(home_clicked);
    //SIGNAL_FUNC_DECLARE(stop_clicked);
private:
    Browser* m_container;
    Evas_Object* m_entry;
    Evas_Object* m_backButton;
    Evas_Object* m_forwardButton;
};

#endif
