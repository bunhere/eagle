/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef WebView_h
#define WebView_h

#include <EflWrappers/Object.h>

class WebView : public Object
{
public:
    WebView(Object* container);

    void loadUrl(const char* url);
private:

    //static WebView* head;
    //WebView* next;
};

#endif
