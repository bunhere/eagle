/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "ShortCut.h"
#include "Browser.h"

static inline bool isSmallAlphabet(char key)
{
    return (key >= 'a' && key <= 'z');
}

static inline bool isNumeric(char key)
{
    return (key >= '0' && key <= '9');
}

ShortCut& ShortCut::instance()
{
    static ShortCut* shortcut = new ShortCut;

    return *shortcut;
}

bool ShortCut::addCommand(char key, bool ctrlPressed, bool altPressed, Command fn)
{
    if (isSmallAlphabet(key) || isNumeric(key))  {
        int modifier = altPressed << 1 + ctrlPressed - 1;

        int index;

        if (isSmallAlphabet(key))
            index = key - 'a';
        else
            index = key - '0';

        m_keyboardShortCuts[modifier][index] = fn;
    }

}

bool ShortCut::addCommand(const char* key, bool ctrlPressed, bool altPressed, Command fn)
{
}

bool ShortCut::feedKeyDownEvent(const Evas_Event_Key_Down& ev, Browser* browser, BrowserContent* content)
{
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(ev.modifiers, "Control");
    Eina_Bool altPressed = evas_key_modifier_is_set(ev.modifiers, "Alt");

    char key = ev.key[0];
    if (isSmallAlphabet(key) || isNumeric(key))  {
        int modifier = altPressed << 1 + ctrlPressed - 1;

        int index;

        if (isSmallAlphabet(key))
            index = key - 'a';
        else
            index = key - '0';

        if (m_keyboardShortCuts[modifier][index])
            return m_keyboardShortCuts[modifier][index](0, browser, content);
    } else {
        // FIXME : It should be implemented.
        browser->executeShortCut(ev.key, ctrlPressed, altPressed);
        return true;
    }
    return false;
}
