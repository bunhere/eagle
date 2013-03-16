/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "config.h"
#include "Urlbar.h"

#include "Browser.h"
#include "FocusController.h"
#include <Edje.h>
#include <Elementary.h>

static Evas_Object* create_toolbar_button(Evas_Object* parent, const char* iconName)
{
    Evas_Object* button = elm_button_add(parent);

    Evas_Object* icon = elm_icon_add(parent);
    elm_icon_standard_set(icon, iconName);
    evas_object_size_hint_max_set(icon, URLBAR_BUTTON_SIZE, URLBAR_BUTTON_SIZE);
    evas_object_color_set(icon, 40, 100, 40, 128);
    evas_object_show(icon);
    elm_object_part_content_set(button, "icon", icon);
    evas_object_size_hint_min_set(button, URLBAR_BUTTON_SIZE, URLBAR_BUTTON_SIZE);

    return button;
}

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

void Urlbar::onKeyDown(void* data, Evas* e, Evas_Object*, void* event_info)
{
    Urlbar* urlbar= static_cast<Urlbar*>(data);
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down*) event_info;
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(evas_key_modifier_get(e), "Control");
    Eina_Bool altPressed = evas_key_modifier_is_set(evas_key_modifier_get(e), "Alt");

    printf(" [Urlbar::%s entered]\n", __func__);
    ShortCut::instance().feedKeyDownEvent(*ev, urlbar->container(), 0);
}

void Urlbar::onMouseDown(void* data, Evas* e, Evas_Object*, void* event_info)
{
    Urlbar* urlbar= static_cast<Urlbar*>(data);
    FocusController::setFocus(urlbar);
}

void Urlbar::back_clicked(void* data, Evas_Object*, void*)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    Browser::back(0, self->container(), 0);
}

void Urlbar::forward_clicked(void* data, Evas_Object*, void*)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    Browser::forward(0, self->container(), 0);
}

void Urlbar::reload_clicked(void* data, Evas_Object*, void*)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    self->container()->reload();
    printf(" %s \n" , __func__);
}

void Urlbar::home_clicked(void* data, Evas_Object*, void*)
{
    // FIXME
}

/*
void Urlbar::stop_clicked(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    Urlbar* self = static_cast<Urlbar*>(data);
    self->container()->stop();
    printf(" %s \n" , __func__);
}
*/

Urlbar::Urlbar(Browser* container)
    : m_container(container)
    , Object(elm_box_add(container->object()))
{
    elm_box_horizontal_set(object(), true);
    evas_object_size_hint_weight_set(object(), EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(object(), EVAS_HINT_FILL, 0.0);

    /* Back button */
    m_backButton = create_toolbar_button(container->object(), "arrow_left");
    evas_object_smart_callback_add(m_backButton, "clicked", back_clicked, this);
    elm_object_disabled_set(m_backButton, EINA_TRUE);
    evas_object_size_hint_weight_set(m_backButton, 0.0, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_backButton, 0.0, 0.5);
    elm_box_pack_end(object(), m_backButton);
    evas_object_show(m_backButton);

    /* Forward button */
    m_forwardButton = create_toolbar_button(container->object(), "arrow_right");
    evas_object_smart_callback_add(m_forwardButton, "clicked", forward_clicked, this);
    elm_object_disabled_set(m_forwardButton, EINA_TRUE);
    evas_object_size_hint_weight_set(m_forwardButton, 0.0, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_forwardButton, 0.0, 0.5);
    elm_box_pack_end(object(), m_forwardButton);
    evas_object_show(m_forwardButton);

    Evas_Object* layout = elm_layout_add(container->object());
    if (!elm_layout_file_set(layout, Browser::themePath(), "eagle/browser-urlbar")) {
        //FIXME: add error handling
        printf("%s theme path is failed\n", Browser::themePath());
        return;
    }
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

    m_entry = elm_entry_add(object());
    elm_entry_single_line_set(m_entry, true);

    evas_object_size_hint_weight_set(m_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);

    evas_object_event_callback_add(m_entry, EVAS_CALLBACK_KEY_DOWN, onKeyDown, this);
    evas_object_event_callback_add(m_entry, EVAS_CALLBACK_MOUSE_DOWN, onMouseDown, this);

    elm_entry_text_style_user_push(m_entry, "DEFAULT='font_size=16'");
    //connect signal
    //evas_object_smart_callback_add(m_entry, "changed", urlbarChanged, this);
    evas_object_smart_callback_add(m_entry, "activated", urlbarActivated, this);

    elm_object_part_content_set(layout, "sw.url", m_entry);
    elm_box_pack_end(object(), layout);
    evas_object_show(layout);

    /* Refresh button */
    Evas_Object* button = create_toolbar_button(container->object(), "refresh");
    evas_object_smart_callback_add(button, "clicked", reload_clicked, this);
    evas_object_size_hint_weight_set(button, 0.0, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(button, 1.0, 0.5);
    elm_box_pack_end(object(), button);
    evas_object_show(button);

    /* Home button */
    button = create_toolbar_button(container->object(), "home");
    evas_object_smart_callback_add(button, "clicked", home_clicked, this);
    evas_object_size_hint_weight_set(button, 0.0, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(button, 1.0, 0.5);
    elm_box_pack_end(object(), button);
    evas_object_show(button);
}

void Urlbar::changeUrlEntry(const char* url)
{
    if (url)
        elm_object_text_set(m_entry, url);
    else
        elm_object_text_set(m_entry, "about:blank");
}

void Urlbar::updateBackFordwardButton(bool backPossible, bool forwardPossible)
{
    elm_object_disabled_set(m_backButton, !backPossible);
    elm_object_disabled_set(m_forwardButton, !forwardPossible);
}

void Urlbar::focusAndSelectAll()
{
    FocusController::setFocus(this);
    elm_entry_select_all(m_entry);
}
