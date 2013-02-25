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

#define HTTP_SCHEME "http"
#define HTTPS_SCHEME "https"

const unsigned MAX_SCHEME_SIZE = 10;

static bool isWellknownScheme(const char* url)
{
    return !strncmp(url, "about", 5)
        || !strncmp(url, "data", 4);
}

static bool foundDomainStyle(const char* url)
{
    if (!strncmp(url, "www.", 4))
        return true;

    int i = 0;
    while (!url[i]) {
        if (url[i] == '.')
            return true;

        if (url[i] == '/')
            return false;
    }
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
        } else if (foundDomainStyle(rawUrl)) {
            schemeBuffer = HTTP_SCHEME;
            m_schemeSize = strlen(HTTP_SCHEME);
            eina_strbuf_string_free(buf);
            eina_strbuf_append_printf(buf, HTTP_SCHEME "://%s", rawUrl);
        } else {
            // search the rawUrl
            schemeBuffer = HTTPS_SCHEME;
            m_schemeSize = strlen(HTTPS_SCHEME);
            eina_strbuf_string_free(buf);

            // FIXME : give a way to choose search engine
            eina_strbuf_append_printf(buf, HTTPS_SCHEME "://www.google.com/search?q=%s", rawUrl);
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
