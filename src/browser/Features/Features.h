/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Features_h
#define Features_h

class AutoFormFill;

class Features
{
public:
    static Features& instance();

    void initialize();

    AutoFormFill* autoFormFill() { return m_autoFormFill; }
private:
    Features();

    AutoFormFill* m_autoFormFill;
};
#endif
