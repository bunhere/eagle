/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef ShortCut_h
#define ShortCut_h

#include <Evas.h>
#include <map>
#include <string>

class Browser;
class BrowserContent;

#define COMMAND_DECLARE(name) \
    static bool name(const ShortCut::CommandInfo*, Browser* browser, BrowserContent* content)

class ShortCut
{
public:
    enum SKEY {
        NONE = 0,
        CTRL = 1,
        ALT = 1 << 1,
        SHIFT = 1 << 2
    };

    class CommandInfo {
    public:
        CommandInfo(const char* key, unsigned skey)
            : m_key(key)
            , m_skey(skey)
        {
        }
        ~CommandInfo()
        {
        }
        const char* key() const { return m_key; }
        unsigned skey() const { return m_skey; }

     private:
        const char* m_key;
        unsigned m_skey;
    };

    static ShortCut& instance();
    virtual ~ShortCut() { }

    typedef bool (*Command)(const ShortCut::CommandInfo*, Browser*, BrowserContent*);

    bool addCommand(char key, SKEY, Command);
    bool addCommand(const char* key, SKEY, Command);

    bool process(const char* text, bool ctrlPressed, bool altPressed, Browser* browser, BrowserContent* content);
    bool feedKeyDownEvent(const Evas_Event_Key_Down& ev, Browser*, BrowserContent*);
private:
    ShortCut();

    // 0: ctrlPressed / 1: altPressed / 2: both
    // 0 ~ 9 : number, 10 ~ 36 : alphabet
    Command m_keyboardAlphabetShortCuts[4][36];
    std::map<std::string, Command*> m_keyboardOtherShortCuts;
};

#endif // #ifndef ShortCut_h
