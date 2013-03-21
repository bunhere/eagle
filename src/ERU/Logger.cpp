/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined(ENABLE_LOG) && ENABLE_LOG
namespace ERU {

void ERULog(const char* file, int line, const char* function, const char* format, ...)
{
    fprintf(stderr, "[%s : %d]%s:", file, line, function);

    va_list args;
    va_start(args, format);
    va_end(args);

    fprintf(stderr, format, args);

    int formatLength = strlen(format);
    if (!formatLength || format[formatLength - 1] == '\n')
        fprintf(stderr, "\n");
}

}
#endif
