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
    MultitabBar(Browser* container);

    void update();
private:
    Browser* m_container;
};

#endif
