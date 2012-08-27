/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include "Browser/Browser.h"
#include "Browser/Features/Features.h"
#include <Elementary.h>

EAPI_MAIN int
elm_main(int argc, char** argv)
{
    Features::instance().initialize();
    Browser* defaultWindow = Browser::create();
    if (!defaultWindow)
        return -1;

    defaultWindow->setTitle("Eagle");
    defaultWindow->show();

    const char* defaultURL;
    if (argc > 1)
        defaultURL = argv[1];
    else
        defaultURL = "http://google.com";

    defaultWindow->loadUrl(defaultURL);

    elm_run();
    elm_shutdown();
    return 0;
}
ELM_MAIN()
