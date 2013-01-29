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

static inline int modifierIndex(unsigned skey)
{
    return skey - 1;
}

static inline int keyIndex(char key)
{
    if (isSmallAlphabet(key))
        return key - 'a' + 10;

    return key - '0';
}

ShortCut& ShortCut::instance()
{
    static ShortCut* shortcut = new ShortCut;

    return *shortcut;
}

ShortCut::ShortCut()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 36; ++j)
            m_keyboardAlphabetShortCuts[i][j] = 0;
}

bool ShortCut::addCommand(char key, SKEY skey, Command fn)
{
    if (isSmallAlphabet(key) || isNumeric(key))  {
        int modifier = modifierIndex(skey);

        int index = keyIndex(key);
        m_keyboardAlphabetShortCuts[modifier][index] = fn;
    }

    return true;
}

bool ShortCut::addCommand(const char* key, SKEY skey, Command fn)
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

    int modifier = modifierIndex(skey);
    if (modifier < 0)
        return false;

    commands[modifier] = fn;
    return true;
}

bool ShortCut::feedKeyDownEvent(const Evas_Event_Key_Down& ev, Browser* browser, BrowserContent* content)
{
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(ev.modifiers, "Control");
    Eina_Bool altPressed = evas_key_modifier_is_set(ev.modifiers, "Alt");

    char key = ev.key[0];
    unsigned skey = ctrlPressed ? CTRL : NONE;
    skey |= altPressed ? ALT : NONE;

    printf("[%s %d, %d]\n", ev.key, altPressed, ctrlPressed);
    if (isSmallAlphabet(key) || isNumeric(key))  {
        int modifier = modifierIndex(skey);
        if (modifier < 0)
            return false;

        int index = keyIndex(key);

        CommandInfo cmdInfo(ev.key, skey);
        if (m_keyboardAlphabetShortCuts[modifier][index])
            return m_keyboardAlphabetShortCuts[modifier][index](&cmdInfo, browser, content);
    } else {
        std::map<std::string, Command*>::iterator i = m_keyboardOtherShortCuts.find(ev.key);
        if (i == m_keyboardOtherShortCuts.end())
            return false;

        int modifier = modifierIndex(skey);
        if (modifier < 0)
            return false;
        
        if (Command cmd = i->second[modifier]) {
            CommandInfo cmdInfo(ev.key, skey);
            return cmd(&cmdInfo, browser, content);
        }
    }
    return false;
}
