/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <Elementary.h>
#include <browser/WebView.h>

#include <browser/Browser.h>
#include <browser/Urlbar.h>

#if USE_WEBKIT
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

#if USE_WEBKIT
    const char* title = static_cast<const Ewk_Text_With_Direction*>(eventInfo)->string;
#else
    const char* title = static_cast<const char*>(eventInfo);
#endif
    toWebView(userData)->container()->setTitle(title);
}

void WebView::onUriChanged(void *userData, Evas_Object *webView, void *eventInfo)
{
    Urlbar& urlbar = toWebView(userData)->container()->urlbar();
    urlbar.changeUrlEntry(static_cast<const char*>(eventInfo));
}

void WebView::onLoadError(void *userData, Evas_Object *webView, void *eventInfo)
{
    printf(" %s \n", __func__);
#if USE_WEBKIT
    Ewk_Frame_Load_Error* error = static_cast<Ewk_Frame_Load_Error*>(eventInfo);
    printf(" %d %d (%s : %s : %s)\n %d\n",
            error->code,
            error->is_cancellation,
            error->domain,
            error->description,
            error->failing_url,
            error->resource_identifier);
#else
    Ewk_Web_Error* error = static_cast<Ewk_Web_Error*>(eventInfo);
    printf(" %d %d (%d : %s : %s)\n",
            ewk_web_error_code_get(error),
            ewk_web_error_cancellation_get(error),
            ewk_web_error_type_get(error),
            ewk_web_error_description_get(error),
            ewk_web_error_url_get(error));
    ewk_web_error_free(error);
#endif
}

void WebView::onKeyDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down*) event_info;
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(evas_key_modifier_get(e), "Control");

    if (ctrlPressed) {
        if (!strcmp(ev->key, "i")) {
            // FIXME: we need better way to handle setting.
#if USE_WEBKIT
            ewk_view_setting_enable_developer_extras_set(ewkObject, true);
            ewk_view_web_inspector_show(ewkObject);
#endif
        } else if (!strcmp(ev->key, "KP_Add")) {
            double ratio = ewk_view_scale_get(ewkObject);
            ewk_view_scale_set(ewkObject, ratio + 0.1, 0, 0);
        } else if (!strcmp(ev->key, "KP_Subtract")) {
            double ratio = ewk_view_scale_get(ewkObject);
            ewk_view_scale_set(ewkObject, ratio - 0.1, 0, 0);
        }
    }
}

void WebView::onMouseDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    evas_object_focus_set(ewkObject, true);
}

WebView::WebView(Browser* container)
    : m_container(container)
{
    Evas* evas = evas_object_evas_get(container->object());
    Evas_Object* ewkView = ewkViewAdd(container->object(), this);

    setObject(ewkView);

    evas_object_size_hint_weight_set(ewkView, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    evas_object_event_callback_add(ewkView, EVAS_CALLBACK_KEY_DOWN, onKeyDown, this);
    evas_object_event_callback_add(ewkView, EVAS_CALLBACK_MOUSE_DOWN, onMouseDown, this);

#define SMART_CALLBACK_ADD(signal, func) \
    evas_object_smart_callback_add(ewkView, signal, func, this)

    SMART_CALLBACK_ADD("inspector,view,create", onInspectorViewCreate);
    SMART_CALLBACK_ADD("inspector,view,close", onInspectorViewClose);
    SMART_CALLBACK_ADD("title,changed", onTitleChanged);
    SMART_CALLBACK_ADD("uri,changed", onUriChanged);
    SMART_CALLBACK_ADD("load,error", onLoadError);
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
    ewk_view_uri_set(object(), url);
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

void WebView::setInspectorView(const WebView& view)
{
#if USE_WEBKIT
    ewk_view_web_inspector_view_set(object(), view.object());
#endif
}
