/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef BrowserContent_h
#define BrowserContent_h

#include "EflWrappers/Object.h"

enum BrowserContentType { BC_WEBVIEW, BC_EMPTY };

class Browser;
class Tab;

class BrowserContent : public Object
{
public:
    BrowserContent(Browser* b, Evas_Object* o, BrowserContentType t) : Object(o), m_container(b), m_type(t), m_tab(0), m_title(0) { }
    virtual ~BrowserContent();

    Browser* container() { return m_container; };

    BrowserContentType type() { return m_type; }
    bool isWebView() { return type() == BC_WEBVIEW; }

    const char* title() const { return m_title; }
    void setTitle(const char*);
    virtual void setFocus(bool) { }

    virtual const char* url() const { return 0; }

    Tab* tab() { return m_tab; }
    void createTabIfNeeded();

private:
    Browser* m_container;
    BrowserContentType m_type;
    Tab* m_tab;
    char* m_title;
};

#endif
