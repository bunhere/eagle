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
    ~WebView();

    static WebView* create(Object* container);

    static void initialize();

    static void onMouseDown(void* data, Evas* e, Evas_Object* webview, void* event_info);

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();
private:
    WebView(Object* container);
};

#endif
