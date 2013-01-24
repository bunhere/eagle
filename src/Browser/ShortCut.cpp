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

        m_keyboardAlphabetShortCuts[modifier][index] = fn;
    }

}

bool ShortCut::addCommand(const char* key, bool ctrlPressed, bool altPressed, Command fn)
{
    Command* commands;
    std::map<std::string, Command*>::iterator i = m_keyboardOtherShortCuts.find(key);
    if (i == m_keyboardOtherShortCuts.end()) {
        commands = new Command[3];
        commands[0] = 0;
        commands[1] = 0;
        commands[2] = 0;
        m_keyboardOtherShortCuts[key] = commands;
    } else
        commands = i->second;

    int modifier = altPressed << 1 + ctrlPressed - 1;
    commands[modifier] = fn;
}

bool ShortCut::feedKeyDownEvent(const Evas_Event_Key_Down& ev, Browser* browser, BrowserContent* content)
{
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(ev.modifiers, "Control");
    Eina_Bool altPressed = evas_key_modifier_is_set(ev.modifiers, "Alt");

    char key = ev.key[0];
    printf("[%s]\n", ev.key);
    if (isSmallAlphabet(key) || isNumeric(key))  {
        int modifier = altPressed << 1 + ctrlPressed - 1;

        int index;

        if (isSmallAlphabet(key))
            index = key - 'a';
        else
            index = key - '0';

        if (m_keyboardAlphabetShortCuts[modifier][index])
            return m_keyboardAlphabetShortCuts[modifier][index](0, browser, content);
    } else {
        fprintf(stderr, "%s\n", __func__);
        std::map<std::string, Command*>::iterator i = m_keyboardOtherShortCuts.find(ev.key);
        if (i == m_keyboardOtherShortCuts.end())
            return false;

        fprintf(stderr, "%s 2\n", __func__);
        int modifier = altPressed << 1 + ctrlPressed - 1;
        
        if (Command cmd = i->second[modifier])
            return cmd(0, browser, content);
    }
    return false;
}
