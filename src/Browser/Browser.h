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
    BrowserContent(Evas_Object* o, BrowserContentType t) : Object(o), m_type(t), m_title(0) { }
    virtual ~BrowserContent();

    BrowserContentType type() { return m_type; }
    bool isWebView() { return type() == BC_WEBVIEW; }

    const char* title() const { return m_title; }
    void setTitle(const char*);
    Tab* tab() { return m_tab; }
    void createTabIfNeeded();

private:
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

    void attachContent(BrowserContent*);
    void detachContent(BrowserContent*);
    size_t contentsSize() const { return m_contents.size(); }
    BrowserContent* contentsAt(size_t i) { return m_contents[i]; }

    void loadUrl(const char* url);
    void back();
    void forward();
    void reload();
    void stop();

    void setInspector(const WebView*);
    void executeShortCut(const char* key, bool ctrlPressed, bool altPressed);
    static const char* themePath();

    virtual void resize(int width, int height);

    void updateMultitab();
    Urlbar* urlbar() { return m_urlbar; }
private:
    explicit Browser(const BrowserConfig&);

    void chooseContent(BrowserContent*);

    BrowserContent* m_content;
    Urlbar* m_urlbar;
    MultitabBar* m_multitapBar;

    Evas_Object* m_layout;
    std::vector<BrowserContent*> m_contents;
};

#endif
