/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef FocusController_h
#define FocusController_h

class Object;
class WebView;

class FocusController
{
public:
    static void setFocus(Object*);
    static void setFocus(WebView*);
    static void lostFocus(WebView*);
};

#endif

