/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Features.h"

AutoFormFill* Features::s_autoFormFill = 0;
DatabaseHandle* Features::s_databaseHandle = 0;

void Features::flush()
{
    if (s_autoFormFill) {
        delete s_autoFormFill;
    }
}

DatabaseHandle::DatabaseHandle()
{
    if (sqlite3_open("./test.db", &m_database) != SQLITE_OK)
        fprintf(stderr, "sqlite3_open error : %s\n", sqlite3_errmsg(m_database));
}

DatabaseHandle::~DatabaseHandle()
{
    sqlite3_close(m_database);
}
