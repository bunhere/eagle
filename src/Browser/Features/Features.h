/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Features_h
#define Features_h

#include "AutoFormFill.h"
#include <sqlite3.h>

class DatabaseHandle
{
public:
    DatabaseHandle();
    ~DatabaseHandle();

    sqlite3* db() const { return m_database; }
private:
    sqlite3* m_database;
};

class Features
{
public:
    static void flush();

    inline static DatabaseHandle* featureDatabase() {
        if (!s_databaseHandle)
            s_databaseHandle = new DatabaseHandle();
        return s_databaseHandle;
    }
    inline static AutoFormFill* autoFormFill() {
        if (!s_autoFormFill)
            s_autoFormFill = new AutoFormFill(featureDatabase());
        return s_autoFormFill;
    }
private:
    static AutoFormFill* s_autoFormFill;
    static DatabaseHandle* s_databaseHandle;
};
#endif
