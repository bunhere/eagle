/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef AutoFormFill_h
#define AutoFormFill_h

#if USE_WEBKIT
#include <EWebKit.h>
#else
#include <EWebKit2.h>
#endif

typedef struct sqlite3 sqlite3;
typedef struct _Eina_List Eina_List;

class AutoFormFill
{
public:
    AutoFormFill();
    ~AutoFormFill();

    void initialize();

    void loadSavedSites();
    bool existURI(const char* uri);
#if USE_WEBKIT
#else
    void saveFormValues(const char* uri, Ewk_Form_Submission_Request*);
#endif
private:

    sqlite3* m_database;
    Eina_List* m_uriList;
};
#endif
