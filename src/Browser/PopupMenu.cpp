/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser/PopupMenu.h"
#include <EWebKit2.h>
#include <Elementary.h>

PopupMenu& PopupMenu::instance()
{
    static PopupMenu popup;

    return popup;
}

PopupMenu::PopupMenu()
{
}

Eina_Bool closeMe(void *data)
{
    printf("%s\n", __func__);
    ewk_view_popup_menu_close(static_cast<Evas_Object*>(data));
    return false;
}

void PopupMenu::menuItemSelected(void* data, Evas_Object* obj, void* event_info)
{
    printf("%s\n", __func__);
    PopupMenu* self = static_cast<PopupMenu*>(data);
    if (!self || !self->m_currentView)
        return;

    Elm_Object_Item* selected = elm_list_selected_item_get(obj);
    
    void* it;

    const Eina_List* l;
    const Eina_List* items;
    int i = 0;
    items = elm_list_items_get(obj);
    EINA_LIST_FOREACH(items, l, it) {
        if(it == selected)
            break;
        i++;
    }

    ewk_view_popup_menu_select(self->m_currentView, i);

    // When I call ewk_view_popup_menu_close directly, crash occurs.
    ecore_idler_add(closeMe, self->m_currentView);
}

void PopupMenu::create(Evas_Object* ewkView, Eina_Rectangle rect, Eina_List* items, int selectedIndex)
{
    printf("%s\n", __func__);
    if (!object())
        destroy();

    m_currentView = ewkView;

    // FIXME : It should be genlist for performance.
    Evas_Object* popupList = elm_list_add(ewkView);
    setObject(popupList);

    evas_object_size_hint_weight_set(popupList, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(popupList, EVAS_HINT_FILL, EVAS_HINT_FILL);

    void* itemv;
    const Eina_List* l;

    int cnt = eina_list_count(items);
    int index = 0;
    EINA_LIST_FOREACH(items, l, itemv) {
        Ewk_Popup_Menu_Item* menuItem = static_cast<Ewk_Popup_Menu_Item*>(itemv);

        const char* text = ewk_popup_menu_item_text_get(menuItem);
        Elm_Object_Item* itemObject = elm_list_item_append(popupList, text, 0, 0, 0, 0);
        if (selectedIndex == index)
            elm_list_item_selected_set(itemObject, true);
        index++;
    }

    evas_object_smart_callback_add(popupList, "selected", menuItemSelected, this);

    int viewX = 0, viewY = 0;
    evas_object_geometry_get(ewkView, &viewX, &viewY, 0, 0);

    move(viewX + rect.x, viewY + rect.y + rect.h);
    resize(rect.w, rect.h * cnt);
    show();
}

void PopupMenu::destroy()
{
    if (!object())
        return;

    evas_object_del(object());
    setObject(0);
}
