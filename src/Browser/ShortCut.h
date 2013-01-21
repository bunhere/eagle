/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef ShortCut_h
#define ShortCut_h

#include <Evas.h>

typedef struct {
    char* key;
    bool ctrlPressed : 1;
    bool altPressed : 1;
    bool shiftPressed : 1;
} CommandInfo;

class Browser;
class BrowserContent;

typedef bool (*Command)(const CommandInfo*, Browser* browser, BrowserContent* content);

#define COMMAND_DECLARE(name) \
    static bool name(const CommandInfo*, Browser* browser, BrowserContent* content)

class ShortCut
{
public:
    static ShortCut& instance();
    virtual ~ShortCut() { }

    bool addCommand(char key, bool ctrlPressed, bool altPressed, Command);
    bool addCommand(const char* key, bool ctrlPressed, bool altPressed, Command);

    bool feedKeyDownEvent(const Evas_Event_Key_Down& ev, Browser*, BrowserContent*);
private:
    ShortCut() { }

    // 0: ctrlPressed / 1: altPressed / 2: both
    // 0 ~ 9 : number, 10 ~ 36 : alphabet
    Command m_keyboardShortCuts[3][36];
};

#endif // #ifndef ShortCut_h
