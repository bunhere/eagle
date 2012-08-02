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

class Browser;

class WebView : public Object
{
public:
    ~WebView();

    static WebView* create(Browser* container);

    static void initialize();

    Browser* container() { return m_container; };

    SMART_CALLBACK_DECLARE(onInspectorViewCreate);
    SMART_CALLBACK_DECLARE(onInspectorViewClose);
    SMART_CALLBACK_DECLARE(onTitleChanged);
    SMART_CALLBACK_DECLARE(onUriChanged);

    static void onKeyDown(void* data, Evas* e, Evas_Object* webview, void* event_info);
    static void onMouseDown(void* data, Evas* e, Evas_Object* webview, void* event_info);

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();

    void setInspectorView(const WebView& view);
private:
    WebView(Browser* container);

    Browser* m_container;
};

#endif
