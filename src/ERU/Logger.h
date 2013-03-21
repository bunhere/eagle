/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef Logger_h
#define Logger_h

namespace ERU {

#if defined(ENABLE_LOG) && ENABLE_LOG
#define LOG(...) ERU::ERULog(__FILE__, __LINE__, __func__, __VA_ARGS__)
void ERULog(const char* file, int line, const char* function, const char* format, ...);
#else
#define LOG(...) ((void)0)
#endif

} // namespace ERU
#endif
