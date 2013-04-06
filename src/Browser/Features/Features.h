/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Features_h
#define Features_h

#include "AutoFormFill.h"
#include "HistoryManager.h"
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
    inline static HistoryManager* historyManager() {
        if (!s_historyManager)
            s_historyManager = new HistoryManager(featureDatabase());
        return s_historyManager;
    }
private:
    static DatabaseHandle* s_databaseHandle;

    static AutoFormFill* s_autoFormFill;
    static HistoryManager* s_historyManager;
};
#endif
