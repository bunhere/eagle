/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <gtest/gtest.h>
#include "ERU/ERUUrl.h"

inline void checkUrl(const char* rawUrl, const char* scheme, const char* prettyUrl)
{
    ERU::Url url(rawUrl);
    EXPECT_STREQ(scheme, url.scheme());
    EXPECT_STREQ(prettyUrl, url.baseUrl());
}

TEST(URLTest, url)
{
    ERU::Url nullUrl(0);
    EXPECT_FALSE(nullUrl.scheme());
    EXPECT_FALSE(nullUrl.baseUrl());

    ERU::Url emptyUrl("");
    EXPECT_FALSE(emptyUrl.scheme());
    EXPECT_FALSE(emptyUrl.baseUrl());

    checkUrl("about:blank", "about", "about:blank");
    checkUrl("http://test.com", "http", "http://test.com");
    checkUrl("test.com", "http", "http://test.com");

    checkUrl("ftp://test.com", "ftp", "ftp://test.com");

    checkUrl("data:text/html", "data", "data:text/html");
}
