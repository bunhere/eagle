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
#include <vector>

class Browser;
class Urlbar;
class MultitabBar;
class Tab;
class WebView;

class BrowserConfig
{
public:
    BrowserConfig();
    bool urlbar;
    bool multitapBar;
};

enum BrowserContentType { BC_WEBVIEW, BC_EMPTY };

class BrowserContent : public Object
{
public:
    BrowserContent(Browser* b, Evas_Object* o, BrowserContentType t) : Object(o), m_container(b), m_type(t), m_title(0) { }
    virtual ~BrowserContent();

    Browser* container() { return m_container; };

    BrowserContentType type() { return m_type; }
    bool isWebView() { return type() == BC_WEBVIEW; }

    const char* title() const { return m_title; }
    void setTitle(const char*);

    virtual const char* url() const { return "about:blank"; }

    Tab* tab() { return m_tab; }
    void createTabIfNeeded();

private:
    Browser* m_container;
    BrowserContentType m_type;
    Tab* m_tab;
    char* m_title;
};

class Browser : public Window
{
public:
    static void initialize();
    static Browser* create(const BrowserConfig& config);
    static Browser* create();

    ~Browser();

    void addPage();

    void attachContent(BrowserContent*, bool);
    void detachContent(BrowserContent*);
    size_t contentsSize() const { return m_contents.size(); }
    BrowserContent* contentsAt(size_t i) { return m_contents[i]; }
    bool isActiveContent(BrowserContent* bc) { return bc == m_content; }
    void chooseContent(BrowserContent*);

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();

    void titleChanged(BrowserContent* content);
    void urlChanged(BrowserContent* content);

    void setInspector(const WebView*);
    void executeShortCut(const char* key, bool ctrlPressed, bool altPressed);
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
