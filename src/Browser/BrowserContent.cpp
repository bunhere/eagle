/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "BrowserContent.h"

#include "Browser.h"
#include "Tab.h"

BrowserContent::~BrowserContent()
{
    if (m_title)
        free(m_title);

    if (m_tab)
        delete m_tab;
}

void BrowserContent::setTitle(const char* title)
{
    if (!title || !title[0])
        return;

    if (m_title)
        free(m_title);

    m_title = strdup(title);

    if (m_tab) {
        m_tab->setTitle(m_title);
    }

    m_container->titleChanged(this);
}

void BrowserContent::createTabIfNeeded()
{
    if (m_tab)
        return;

    m_tab = new Tab(this);

    if (m_container->isActiveContent(this))
        m_tab->setActive(true);
}

