/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Tab_h
#define Tab_h

#include <EflWrappers/Object.h>

class BrowserContent;

class Tab : public Object
{
public:
    Tab(BrowserContent* container);

    void setActive(bool);
    void setTitle(const char* title);
private:
    Evas_Object* m_title;
    bool m_active;
};

#endif

