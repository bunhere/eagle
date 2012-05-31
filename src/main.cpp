/*
 * Copyright (C) 2012 Ryuan Choi
 * FIXME: Need to decide license.
 * Now almost code is from MiniBrowser/Efl
 */

#include <ewk_view.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Eina.h>
#include <Evas.h>

static const int WIDTH = 800;
static const int HEIGHT = 600;

int main()
{
    if (!ecore_evas_init())
        return 0;

    Ecore_Evas* ee = ecore_evas_new(0, 0, 0, WIDTH, HEIGHT, 0);

    ecore_evas_title_set(ee, "Eagle - WebKit2/Efl based test browser");
    ecore_evas_show(ee);

    Evas* evas = ecore_evas_get(ee);

    Evas_Object* bg = evas_object_rectangle_add(evas);
    evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    evas_object_move(bg, 0, 0);
    evas_object_resize(bg, WIDTH, HEIGHT);
    evas_object_color_set(bg, 255, 255, 255, 255);
    evas_object_show(bg);

    /* Create webview */
    Evas_Object* webview = ewk_view_add(evas);
    evas_object_size_hint_weight_set(webview, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_resize(webview, WIDTH, HEIGHT);
    evas_object_show(webview);

    ewk_view_uri_set(webview, "http://enlightenment.org");

    ecore_main_loop_begin();
    ecore_evas_shutdown();
    return 0;
}
