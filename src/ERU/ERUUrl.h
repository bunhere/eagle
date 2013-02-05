/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#ifndef ERUUrl_h
#define ERUUrl_h

namespace ERU {

class Url {
public:
    explicit Url(const char* rawUrl);
    ~Url();

    const char* domain() const { return m_buffer + m_schemeSize; }
    const char* scheme() const { return m_buffer; }
    const char* baseUrl() const { return m_url; }
private:
    unsigned atScheme(const char* rawUrl);

    char* m_buffer;
    char* m_url;

    int m_schemeSize;
    bool m_isValid;
};

} // namespace ERU

#endif
