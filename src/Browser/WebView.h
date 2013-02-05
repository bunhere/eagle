/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef WebView_h
#define WebView_h

#include "Browser.h"
#include "ERU/ERUUrl.h"

class WebView : public BrowserContent
{
public:
    ~WebView();

    static WebView* create(Browser* container);

    static void initialize();

    void setFocus(bool);

    const char* url() const { return m_url->baseUrl(); }

    SMART_CALLBACK_DECLARE(onFormSubmissionRequest);

    SMART_CALLBACK_DECLARE(onInspectorViewCreate);
    SMART_CALLBACK_DECLARE(onInspectorViewClose);

    SMART_CALLBACK_DECLARE(onLoadError);
    SMART_CALLBACK_DECLARE(onLoadFinished);

    SMART_CALLBACK_DECLARE(onTitleChanged);
    SMART_CALLBACK_DECLARE(onUriChanged);

    //static void onKeyDown(void* data, Evas* e, Evas_Object* webview, void* event_info);
    bool smartKeyDown(const Evas_Event_Key_Down*);
    static void onMouseDown(void* data, Evas* e, Evas_Object* webview, void* event_info);

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();

    void openInspectorView();

    void scaleUp();
    void scaleDown();
private:
    WebView(Browser* container);

    WebView* m_inspector;
    ERU::Url* m_url;
};

#if !USE_ELM_WEB
Evas_Object* ewkViewAdd(Evas_Object* parent, WebView* webView);
#endif

#endif
