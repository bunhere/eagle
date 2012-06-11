/*
 * eagle
 *
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <browser/Browser.h>
#include <Elementary.h>

EAPI_MAIN int
elm_main(int argc, char** argv)
{
    Browser* defaultWindow = Browser::create();
    if (!defaultWindow)
        return -1;

    defaultWindow->setTitle("Eagle");
    defaultWindow->show();

    defaultWindow->loadUrl("http://enlightenment.org");

    elm_run();
    elm_shutdown();
    return 0;
}
ELM_MAIN()
