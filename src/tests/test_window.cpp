/*
 * Copyright (C) 2012 Ryuan Choi
 *
 * License LGPL-3, see COPYING file at project folder.
 */

#include <gtest/gtest.h>
#include <Elementary.h>
#include "EflWrappers/Window.h"

TEST(WindowTest, test)
{
    elm_init(0, 0);
    Window* w1 = new Window(100, 100);

    Window::destroy(w1->object());

    w1 = new Window(100, 100);
    Window* w2 = new Window(100, 100);
    Window::destroy(w1->object());
    Window::destroy(w2->object());

    w1 = new Window(100, 100);
    w2 = new Window(100, 100);
    Window::destroy(w2->object());
    Window::destroy(w1->object());

    w1 = new Window(100, 100);
    w2 = new Window(100, 100);
    Window* w3 = new Window(100, 100);
    Window::destroy(w2->object());
    Window::destroy(w3->object());
    Window::destroy(w1->object());

    EXPECT_TRUE(true);
}
