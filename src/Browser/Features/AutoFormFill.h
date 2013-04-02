/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef AutoFormFill_h
#define AutoFormFill_h

#if USE_WEBKIT || USE_ELM_WEB
#include <EWebKit.h>
#else
#include <EWebKit2.h>
#endif

typedef struct sqlite3 sqlite3;
typedef struct _Eina_List Eina_List;

class DatabaseHandle;

class AutoFormFill
{
public:
    explicit AutoFormFill(DatabaseHandle*);
    ~AutoFormFill();

    void loadSavedSites();
    bool existURI(const char* uri);
#if USE_WEBKIT || USE_ELM_WEB
#else
    void saveFormValues(const char* uri, Ewk_Form_Submission_Request*);
#endif
private:

    DatabaseHandle* m_handle;
    Eina_List* m_uriList;
};
#endif
