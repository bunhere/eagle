/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef MultitabBar_h
#define MultitabBar_h

#include <EflWrappers/Object.h>

class Browser;
class MultitabBar : public Object
{
public:
    explicit MultitabBar(Browser* container);
    ~MultitabBar();

    Browser* container() { return m_container; };

    void update();

private:
    SMART_CALLBACK_DECLARE(add_clicked);

    Browser* m_container;
    Evas_Object* m_addButton;
};

#endif
