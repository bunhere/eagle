/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "AutoFormFill.h"
#include <sqlite3.h>

#include <stdio.h>

const char SQL_CREATE[] =
    "create table if not exists tbAutoFormFill"
    "("
    " uri text"
    ");"
    "create table if not exists tbAutoFormFillItem"
    "("
    " pid integer,"
    " field text,"
    " value text"
    ");";

const char SQL_LOAD_SAVED_SITES[] =
    "select uri from tbAutoFormFill;";

const char SQL_INSERT_FORMFILL[] =
    "insert into tbAutoFormFill values('%s');";
const char SQL_INSERT_FORMFILLITEM[] =
    "insert into tbAutoFormFillItem values(?, ?, ?);";

void AutoFormFill::initialize()
{
    if (sqlite3_open("./test.db", &m_database) != SQLITE_OK)
        printf("sqlite3_open error : %s\n", sqlite3_errmsg(m_database));

    sqlite3_exec(m_database, SQL_CREATE, 0, 0, 0);

    loadSavedSites();
}

AutoFormFill::AutoFormFill()
    : m_uriList(0)
{
}

AutoFormFill::~AutoFormFill()
{
    sqlite3_close(m_database);
}

void AutoFormFill::loadSavedSites()
{
    char** result;
    char* err;
    int rows;
    int columns;

    if (sqlite3_get_table(m_database, SQL_LOAD_SAVED_SITES, &result, &rows, &columns, &err) != SQLITE_OK)
        return;

    for (int i = 0; i < rows; ++i) {
        m_uriList = eina_list_append(m_uriList, result[i]);
    }
}

bool AutoFormFill::existURI(const char* uri)
{
    Eina_List* l;
    void* data;
    EINA_LIST_FOREACH(m_uriList, l, data) {
        const char* uriItem = (const char*)uri;
        if (!strcmp(uri, uriItem))
            return true;
    }
    return false;
}

#if USE_WEBKIT || USE_ELM_WEB
#else
void AutoFormFill::saveFormValues(const char* uri, Ewk_Form_Submission_Request* request)
{
    sqlite3_exec(m_database, "begin transaction;", 0, 0, 0);
    const char* exeSql = eina_stringshare_printf(SQL_INSERT_FORMFILL, uri);
    sqlite3_exec(m_database, exeSql, 0, 0, 0);

    int pid = sqlite3_last_insert_rowid(m_database);

    sqlite3_stmt* state;
    const char* tail;
    sqlite3_prepare_v2(m_database, SQL_INSERT_FORMFILLITEM, strlen(SQL_INSERT_FORMFILLITEM), &state, &tail);

    Eina_List* list = ewk_form_submission_request_field_names_get(request);

    void* data;
    EINA_LIST_FREE(list, data) {
        const char* field = (char*)data;
        const char* value = ewk_form_submission_request_field_value_get(request, (char*)data);

        sqlite3_bind_int(state, 1, pid);
        sqlite3_bind_text(state, 2, field, strlen(field), SQLITE_STATIC);
        sqlite3_bind_text(state, 3, value, strlen(value), SQLITE_STATIC);

        sqlite3_step(state);
        sqlite3_reset(state);
    }
    sqlite3_finalize(state);

    sqlite3_exec(m_database, "commit transaction;", 0, 0, 0);
}
#endif
