/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Features.h"

#include "AutoFormFill.h"

Features& Features::instance()
{
    static Features features;
    return features;
}

Features::Features()
{
    m_autoFormFill = new AutoFormFill;
}

void Features::initialize()
{
    m_autoFormFill->initialize();
}
