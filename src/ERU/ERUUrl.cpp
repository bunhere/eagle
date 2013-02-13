/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "ERUUrl.h"

#include <Eina.h>
#include <Ecore_File.h>
#include <string.h>

namespace ERU {

const unsigned MAX_SCHEME_SIZE = 10;

static bool isWellknownScheme(const char* url)
{
    return !strncmp(url, "about", 5)
        || !strncmp(url, "data", 4);
}

Url::Url(const char* rawUrl)
{
    if (!rawUrl || !rawUrl[0]) {
        m_buffer = 0;
        m_url = 0;
        m_isValid = false;
        return;
    }

    m_schemeSize = 0;
    const char* schemeBuffer;
    for (unsigned i = 0;i < MAX_SCHEME_SIZE; ++i) {
        if (rawUrl[i] == ':') {
            if ((rawUrl[i + 1] == '/' && rawUrl[i + 2] == '/')
                || isWellknownScheme(rawUrl)) {
                m_schemeSize = i;
                schemeBuffer = rawUrl;
            }

            break;
        } else if (!rawUrl[i])
            break;
    }

    if (!m_schemeSize) {
        Eina_Strbuf* buf = eina_strbuf_manage_new(eina_file_path_sanitize(rawUrl));
        if (ecore_file_exists(eina_strbuf_string_get(buf))) {
            schemeBuffer = "file";
            m_schemeSize = 4;
            eina_strbuf_prepend(buf, "file://");
        } else {
            schemeBuffer = "http";
            m_schemeSize = 4;
            eina_strbuf_string_free(buf);
            eina_strbuf_append_printf(buf, "http://%s", rawUrl);
        }

        m_url = eina_strbuf_string_steal(buf);
        eina_strbuf_free(buf);
    } else
        m_url = strdup(rawUrl);

    unsigned bufferSize;
    bufferSize = m_schemeSize;

    m_buffer = new char[bufferSize];
    strncpy(m_buffer, schemeBuffer, m_schemeSize);
    m_buffer[m_schemeSize] = 0;
}

Url::~Url()
{
    delete[] m_buffer;
    free(m_url);
}

unsigned Url::atScheme(const char* rawUrl)
{
    return 0;
}

} // namespace ERU
