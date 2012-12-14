/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <Elementary.h>
#include <Browser/WebView.h>

#include <Browser/Browser.h>
#include <Browser/Urlbar.h>
#include <Browser/Features/AutoFormFill.h>
#include <Browser/Features/Features.h>

#if USE_WEBKIT || USE_ELM_WEB
#include <EWebKit.h>
#else
#include <EWebKit2.h>
#endif

inline static WebView* toWebView(void* userData)
{
    return static_cast<WebView*>(userData);
}

void WebView::onInspectorViewCreate(void *userData, Evas_Object *webView, void *eventInfo)
{
    toWebView(userData)->container()->createInspector(toWebView(userData));
}

void WebView::onInspectorViewClose(void *userData, Evas_Object *webView, void *eventInfo)
{
    toWebView(userData)->container()->closeInspector();
}

void WebView::onTitleChanged(void *userData, Evas_Object *webView, void *eventInfo)
{
    if (!eventInfo)
        return;

#if USE_WEBKIT || USE_ELM_WEB
    const char* title = static_cast<const Ewk_Text_With_Direction*>(eventInfo)->string;
#else
    const char* title = static_cast<const char*>(eventInfo);
#endif
    toWebView(userData)->container()->setTitle(title);
}

void WebView::onUriChanged(void *userData, Evas_Object *webView, void *eventInfo)
{
    Urlbar* urlbar = toWebView(userData)->container()->urlbar();
    if (urlbar)
        urlbar->changeUrlEntry(static_cast<const char*>(eventInfo));
}

void WebView::onLoadError(void *userData, Evas_Object *webView, void *eventInfo)
{
    printf(" %s \n", __func__);
#if USE_WEBKIT || USE_ELM_WEB
    Ewk_Frame_Load_Error* error = static_cast<Ewk_Frame_Load_Error*>(eventInfo);
    printf(" %d %d (%s : %s : %s)\n %d\n",
            error->code,
            error->is_cancellation,
            error->domain,
            error->description,
            error->failing_url,
            error->resource_identifier);
#else
    Ewk_Error* error = static_cast<Ewk_Error*>(eventInfo);
    printf(" %d %d (%d : %s : %s)\n",
            ewk_error_code_get(error),
            ewk_error_cancellation_get(error),
            ewk_error_type_get(error),
            ewk_error_description_get(error),
            ewk_error_url_get(error));
#endif
}

void WebView::onLoadFinished(void *userData, Evas_Object *webView, void *eventInfo)
{
    printf(" %s \n", __func__);
    AutoFormFill* formFillFeature = Features::instance().autoFormFill();

#if USE_WEBKIT || USE_ELM_WEB
#else
    if (formFillFeature && formFillFeature->existURI(ewk_view_url_get(webView))) {
        // FIXME: we need a way to change contents.
    }
#endif
}

void WebView::onFormSubmissionRequest(void *userData, Evas_Object *webView, void *eventInfo)
{
    printf(" %s \n", __func__);
#if USE_WEBKIT || USE_ELM_WEB
#else
    Ewk_Form_Submission_Request* request = static_cast<Ewk_Form_Submission_Request*>(eventInfo);

    AutoFormFill* formFillFeature = Features::instance().autoFormFill();

    if (formFillFeature) {
        bool needToUpdate = true;

        const char* url = ewk_view_url_get(webView);
        if (formFillFeature->existURI(url)) {
            needToUpdate = false;
        }

        if (needToUpdate)
            formFillFeature->saveFormValues(url, request);
    }

    ewk_form_submission_request_submit(request);
#endif
}

void WebView::onKeyDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down*) event_info;
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(evas_key_modifier_get(e), "Control");

    toWebView(data)->container()->executeShortCut(ev->key, ctrlPressed, false);
}

void WebView::onMouseDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    elm_object_focus_set(elm_object_top_widget_get(toWebView(data)->container()->object()), false);
    evas_object_focus_set(ewkObject, true);
}

WebView::WebView(Browser* container)
    : m_container(container)
{
    Evas* evas = evas_object_evas_get(container->object());
#if USE_ELM_WEB
    Evas_Object* webView = elm_web_add(container->object());
    Evas_Object* ewkView = elm_web_webkit_view_get(webView);

#else
    Evas_Object* ewkView = ewkViewAdd(container->object(), this);
#endif

    setObject(ewkView);

    evas_object_size_hint_weight_set(ewkView, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    evas_object_event_callback_add(ewkView, EVAS_CALLBACK_KEY_DOWN, onKeyDown, this);
    evas_object_event_callback_add(ewkView, EVAS_CALLBACK_MOUSE_DOWN, onMouseDown, this);

#define SMART_CALLBACK_ADD(signal, func) \
    evas_object_smart_callback_add(ewkView, signal, func, this)

    SMART_CALLBACK_ADD("inspector,view,create", onInspectorViewCreate);
    SMART_CALLBACK_ADD("inspector,view,close", onInspectorViewClose);
    SMART_CALLBACK_ADD("title,changed", onTitleChanged);
#if USE_WEBKIT || USE_ELM_WEB
    SMART_CALLBACK_ADD("uri,changed", onUriChanged);
#else
    SMART_CALLBACK_ADD("url,changed", onUriChanged);
#endif
    SMART_CALLBACK_ADD("load,error", onLoadError);
    SMART_CALLBACK_ADD("load,finished", onLoadFinished);

#if USE_WEBKIT
#else
    SMART_CALLBACK_ADD("form,submission,request", onFormSubmissionRequest);
#endif

#undef SMART_CALLBACK_ADD

}

WebView::~WebView()
{
    evas_object_event_callback_del(object(), EVAS_CALLBACK_MOUSE_DOWN, onMouseDown);

    evas_object_smart_callback_del(object(), "inspector,view,create", onInspectorViewCreate);
    evas_object_smart_callback_del(object(), "inspector,view,close", onInspectorViewClose);
}

WebView* WebView::create(Browser* container)
{
    WebView* webview = new WebView(container);

    //FIXME: hard typed path is bad.
    ewk_view_theme_set(webview->object(), "/usr/local/share/ewebkit-0/themes/default.edj");

    return webview;
}

void WebView::initialize()
{
    ewk_init();

#if USE_WEBKIT
    const char* httpProxy = getenv("http_proxy");
    if (httpProxy)
        ewk_network_proxy_uri_set(httpProxy);
#endif
}

void WebView::loadUrl(const char* url)
{
#if USE_WEBKIT || USE_ELM_WEB
    ewk_view_uri_set(object(), url);
#else
    ewk_view_url_set(object(), url);
#endif
}

void WebView::back()
{
    ewk_view_back(object());
}

void WebView::forward()
{
    ewk_view_forward(object());
}

void WebView::reload()
{
    ewk_view_reload(object());
}

void WebView::stop()
{
    ewk_view_stop(object());
}

void WebView::openInspectorView()
{
#if USE_WEBKIT
    // FIXME: we need better way to handle setting.
    ewk_view_setting_enable_developer_extras_set(object(), true);
    ewk_view_web_inspector_show(object());
#endif
}

void WebView::setInspectorView(const WebView& view)
{
#if USE_WEBKIT
    ewk_view_web_inspector_view_set(object(), view.object());
#endif
}

void WebView::scaleUp()
{
    double ratio = ewk_view_scale_get(object());
    ewk_view_scale_set(object(), ratio + 0.1, 0, 0);
}

void WebView::scaleDown()
{
    double ratio = ewk_view_scale_get(object());
    ewk_view_scale_set(object(), ratio - 0.1, 0, 0);
}
