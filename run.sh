#!/bin/bash

# eagle
#                                                     
# Copyright (C) 2012 Ryuan Choi
#                                                     
# License LGPL-3, see COPYING file at project folder.

# simple run script.
help() {
    echo "Usage $0 [options]"

    echo
    echo "Options:"
    echo "  -1                  : run with webkit1 (default is to run with webkit2)"
    echo "  --help              : show this help message and exit"
}

build_directory="WebKit2"

while [ $# -gt 0 ]
do
    if [ "$1" = "-1" ]
    then
        build_directory="WebKit"
    fi
    if [ "$1" = "--help" ]
    then
        help
        exit;
    fi
    shift
done

cd BuildTmp/$build_directory
./eagle
cd ../..
