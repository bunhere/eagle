/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Urlbar.h"

#include "Browser.h"
#include <Edje.h>
#include <Elementary.h>

static void urlbarChanged(void* data, Evas_Object* obj, void* eventInfo)
{
    printf(" [%s]\n", elm_object_text_get(obj));
}

static void urlbarActivated(void* data, Evas_Object* obj, void* eventInfo)
{
    Urlbar* urlbar= static_cast<Urlbar*>(data);
    const char* url = elm_object_text_get(obj);
    printf(" [%s entered]\n", elm_object_text_get(obj));
    urlbar->container()->loadUrl(elm_entry_markup_to_utf8(url));
}

void Urlbar::back_clicked(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    self->container()->back();
}

void Urlbar::forward_clicked(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    self->container()->forward();
}

void Urlbar::reload_clicked(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    self->container()->reload();
    printf(" %s \n" , __func__);
}

void Urlbar::stop_clicked(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    self->container()->stop();
    printf(" %s \n" , __func__);
}

Urlbar::Urlbar(Browser* container)
    : m_container(container)
{
    Evas_Object* obj = elm_layout_add(container->object());

    if (!elm_layout_file_set(obj, container->getTheme(), "eagle/browser-urlbar")) {
        //FIXME: add error handling
        printf("%s theme path is failed\n", container->getTheme());
        return;
    }

    m_entry = elm_entry_add(obj);
    elm_entry_single_line_set(m_entry, true);

    evas_object_size_hint_weight_set(m_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);

    elm_object_part_content_set(obj, "sw.url", m_entry);

    //connect signal
    //evas_object_smart_callback_add(m_entry, "changed", urlbarChanged, this);
    evas_object_smart_callback_add(m_entry, "activated", urlbarActivated, this);

    edje_object_signal_callback_add(elm_layout_edje_get(obj), "mouse,clicked,1", "back", back_clicked, this);
    edje_object_signal_callback_add(elm_layout_edje_get(obj), "mouse,clicked,1", "forward", forward_clicked, this);
    edje_object_signal_callback_add(elm_layout_edje_get(obj), "reload", "", reload_clicked, this);
    edje_object_signal_callback_add(elm_layout_edje_get(obj), "stop", "", stop_clicked, this);

    setObject(obj);
}

Urlbar::~Urlbar()
{
}

void Urlbar::changeUrlEntry(const char* url)
{
    elm_object_text_set(m_entry, url);
}