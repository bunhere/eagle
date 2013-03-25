#!/bin/bash

# eagle
#                                                     
# Copyright (C) 2012 Ryuan Choi
#                                                     
# License LGPL-3, see COPYING file at project folder.

# simple build script.
help() {
    echo "Usage $0 [options]"

    echo
    echo "Options:"
    echo "  -1                  : build with webkit1 (default is to build with webkit2)"
    echo "  --mobile            : build like mobile"
    echo "  --help              : show this help message and exit"
}

options=""
build_directory="WebKit2"

while [ $# -gt 0 ]
do
    if [ "$1" = "-1" ]
    then
        options="$options -DUSE_WEBKIT=On"
        build_directory="WebKit"
    fi
    if [ "$1" = "--mobile" ]
    then
        options="$options -DUSE_MOBILE_PROFILE=On"
    fi
    if [ "$1" = "--help" ]
    then
        help
        exit;
    fi
    shift
done

mkdir -p BuildTmp
cd BuildTmp

mkdir -p $build_directory
cd $build_directory

cmake ../.. $options
make -j4

cd ../..
