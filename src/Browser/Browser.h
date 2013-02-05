/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Browser_h
#define Browser_h

#include "EflWrappers/Window.h"
#include "ShortCut.h"
#include <vector>

class Browser;
class Urlbar;
class MultitabBar;
class WebView;

class BrowserConfig
{
public:
    BrowserConfig();
    bool urlbar;
    bool multitapBar;
};

class Browser : public Window
{
public:
    static void initialize();
    static Browser* create(const BrowserConfig& config);
    static Browser* create();

    static void onFocusIn(void*, Evas*, Evas_Object*, void*);
    static void onFocusOut(void*, Evas*, Evas_Object*, void*);

    ~Browser();

    void addNewPage(BrowserContent* = 0);
    void closePage(BrowserContent* = 0);
    void focusAndSelectUrlBar(BrowserContent* = 0);
    void toggleFullScreen(BrowserContent* = 0);

    void attachContent(BrowserContent*, bool);
    void detachContent(BrowserContent*);
    size_t contentsSize() const { return m_contents.size(); }
    BrowserContent* contentsAt(size_t i) { return m_contents[i]; }
    bool isActiveContent(BrowserContent* bc) { return bc == m_content; }
    void chooseContent(BrowserContent*, bool updateMultitab = true);

    COMMAND_DECLARE(addNewPage);
    COMMAND_DECLARE(closePage);
    COMMAND_DECLARE(focusAndSelectUrlBar);
    COMMAND_DECLARE(movePage);
    COMMAND_DECLARE(openInspectorView);
    COMMAND_DECLARE(scaleUp);
    COMMAND_DECLARE(scaleDown);
    COMMAND_DECLARE(back);
    COMMAND_DECLARE(forward);
    COMMAND_DECLARE(toggleFullScreen);

    void loadUrl(const char* url);
    void reload();
    void stop();

    void titleChanged(BrowserContent*);
    void urlChanged(BrowserContent*);

    void setInspector(const WebView*);
    static const char* themePath();

    virtual void resize(int width, int height);

    void updateMultitab();
private:
    explicit Browser(const BrowserConfig&);

    BrowserContent* m_content;
    Urlbar* m_urlbar;
    MultitabBar* m_multitapBar;

    Evas_Object* m_layout;
    std::vector<BrowserContent*> m_contents;
};

#endif
