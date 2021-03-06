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
#include <Browser/Features/AutoFormFill.h>
#include <Browser/Features/Features.h>
#include "ERU/Logger.h"
#include "FocusController.h"

#if USE_WEBKIT || USE_ELM_WEB
#include <EWebKit.h>
#else
#include <EWebKit2.h>
#endif

inline static WebView* toWebView(void* userData)
{
    return static_cast<WebView*>(userData);
}

void WebView::onInspectorViewCreate(void* userData, Evas_Object*, void*)
{
    WebView* webView = toWebView(userData);
    WebView* inspector = webView->m_inspector;

    if (!inspector) {
        inspector = new WebView(webView->container());
        webView->m_inspector = inspector;

#if USE_WEBKIT
        ewk_view_inspector_view_set(webView->webViewObject(), inspector->webViewObject());
#endif
    }

    webView->container()->setInspector(inspector);
}

void WebView::onInspectorViewClose(void* userData, Evas_Object*, void*)
{
    WebView* webView = toWebView(userData);
    webView->container()->setInspector(0);

    if (webView->m_inspector) {
        delete webView->m_inspector;
        webView->m_inspector = 0;
    }
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

    WebView* self = toWebView(userData);
    self->setTitle(title);
}

void WebView::onUriChanged(void* userData, Evas_Object*, void* eventInfo)
{
    WebView* self = toWebView(userData);
    const char* newUrl = static_cast<const char*>(eventInfo);

    if (self->m_url && !strcmp(self->url(), newUrl))
        return;

    if (self->m_url)
        delete self->m_url;

    self->m_url = new ERU::Url(newUrl);
    self->container()->urlChanged(self);

#if USE_WEBKIT || USE_ELM_WEB
    onBackForwardListChanged(userData, self->webViewObject(), 0);
#endif
}

void WebView::onLoadError(void *userData, Evas_Object *webView, void *eventInfo)
{
    LOG("");
#if USE_WEBKIT || USE_ELM_WEB
    Ewk_Frame_Load_Error* error = static_cast<Ewk_Frame_Load_Error*>(eventInfo);
    LOG(" %d %d (%s : %s : %s)\n %ld\n",
            error->code,
            error->is_cancellation,
            error->domain,
            error->description,
            error->failing_url,
            error->resource_identifier);
#else
    Ewk_Error* error = static_cast<Ewk_Error*>(eventInfo);
    LOG(" %d %d (%d : %s : %s)\n",
            ewk_error_code_get(error),
            ewk_error_cancellation_get(error),
            ewk_error_type_get(error),
            ewk_error_description_get(error),
            ewk_error_url_get(error));
#endif
}

void WebView::onLoadFinished(void *userData, Evas_Object *webView, void *eventInfo)
{
    LOG("");
    AutoFormFill* formFillFeature = Features::autoFormFill();

#if USE_WEBKIT || USE_ELM_WEB
#else
    if (formFillFeature && formFillFeature->existURI(ewk_view_url_get(webView))) {
        // FIXME: we need a way to change contents.
    }
#endif
}

void WebView::onBackForwardListChanged(void *userData, Evas_Object *webView, void *eventInfo)
{
    LOG("");
    WebView* self = toWebView(userData);
    self->container()->backFordwardListChanged(self, self->backPossible(), self->forwardPossible());
}

void WebView::onFormSubmissionRequest(void *userData, Evas_Object *webView, void *eventInfo)
{
    LOG("");
#if USE_WEBKIT || USE_ELM_WEB
#else
    Ewk_Form_Submission_Request* request = static_cast<Ewk_Form_Submission_Request*>(eventInfo);

    AutoFormFill* formFillFeature = Features::autoFormFill();

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

bool WebView::smartKeyDown(const Evas_Event_Key_Down *ev)
{
    return ShortCut::instance().feedKeyDownEvent(*ev, container(), this);
}

void onKeyDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
}

static void onFocusIn(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    LOG("");
    FocusController::setFocus(toWebView(data));
}

void onFocusOut(void* data, Evas*, Evas_Object*, void*)
{
    LOG("");
    FocusController::lostFocus(toWebView(data));
}

void WebView::onMouseDown(void* data, Evas* e, Evas_Object* ewkObject, void* event_info)
{
    WebView* self = toWebView(data);
    self->setFocus(true);
}

WebView::WebView(Browser* container)
    : BrowserContent(container, ewkViewAdd(container->object(), this), BC_WEBVIEW)
    , m_inspector(0)
    , m_url(0)
{

    Evas* evas = evas_object_evas_get(container->object());

    evas_object_size_hint_weight_set(webViewObject(), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    evas_object_event_callback_add(webViewObject(), EVAS_CALLBACK_KEY_DOWN, onKeyDown, this);
    evas_object_event_callback_add(webViewObject(), EVAS_CALLBACK_MOUSE_DOWN, onMouseDown, this);
    evas_object_event_callback_add(webViewObject(), EVAS_CALLBACK_FOCUS_IN, onFocusIn, this);
    evas_object_event_callback_add(webViewObject(), EVAS_CALLBACK_FOCUS_OUT, onFocusOut, this);

#define SMART_CALLBACK_ADD(signal, func) \
    evas_object_smart_callback_add(webViewObject(), signal, func, this)

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

    // signal for only WK2
    SMART_CALLBACK_ADD("back,forward,list,changed", onBackForwardListChanged);

#if USE_WEBKIT
#else
    SMART_CALLBACK_ADD("form,submission,request", onFormSubmissionRequest);
#endif

#undef SMART_CALLBACK_ADD

}

WebView::~WebView()
{
    evas_object_event_callback_del(webViewObject(), EVAS_CALLBACK_MOUSE_DOWN, onMouseDown);

    evas_object_smart_callback_del(webViewObject(), "inspector,view,create", onInspectorViewCreate);
    evas_object_smart_callback_del(webViewObject(), "inspector,view,close", onInspectorViewClose);

    if (m_inspector)
        delete m_inspector;

    if (m_url)
        delete m_url;
}

WebView* WebView::create(Browser* container)
{
    WebView* webView = new WebView(container);

#ifdef WEBKIT_DEFAULT_THEME_PATH
    ewk_view_theme_set(webView->webViewObject(), WEBKIT_DEFAULT_THEME_PATH "/themes/default.edj");
#endif

    return webView;
}

void WebView::initialize()
{
#if USE_WEBKIT2
    // FIXME: WebKit2 should support software backend also.
    elm_config_preferred_engine_set("opengl_x11");
#endif

    ewk_init();

#if USE_WEBKIT
    const char* httpProxy = getenv("http_proxy");
    if (httpProxy)
        ewk_network_proxy_uri_set(httpProxy);
#endif
}

void WebView::setFocus(bool focus)
{
    FocusController::setFocus(this);
}

void WebView::loadUrl(const char* urlToLoad)
{
    if (m_url)
        delete m_url;

    m_url = new ERU::Url(urlToLoad);

#if USE_WEBKIT || USE_ELM_WEB
    ewk_view_uri_set(webViewObject(), url());
#else
    ewk_view_url_set(webViewObject(), url());
#endif
}

void WebView::back()
{
    ewk_view_back(webViewObject());
}

void WebView::forward()
{
    ewk_view_forward(webViewObject());
}

void WebView::reload()
{
    ewk_view_reload(webViewObject());
}

void WebView::stop()
{
    ewk_view_stop(webViewObject());
}

bool WebView::backPossible()
{
    return ewk_view_back_possible(webViewObject());
}

bool WebView::forwardPossible()
{
    return ewk_view_forward_possible(webViewObject());
}

void WebView::openInspectorView()
{
#if USE_WEBKIT
    // FIXME: we need better way to handle setting.
    ewk_view_setting_enable_developer_extras_set(webViewObject(), true);
    ewk_view_inspector_show(webViewObject());
#endif
}

void WebView::setSourceMode()
{
#if !USE_WEBKIT && !USE_ELM_WEB
    fprintf(stderr, "%s\n", __func__);
    ewk_view_source_mode_set(webViewObject(), true);
#endif
}

void WebView::scaleUp()
{
    double ratio = ewk_view_scale_get(webViewObject());
    ewk_view_scale_set(webViewObject(), ratio + 0.1, 0, 0);
}

void WebView::scaleDown()
{
    double ratio = ewk_view_scale_get(webViewObject());
    ewk_view_scale_set(webViewObject(), ratio - 0.1, 0, 0);
}
