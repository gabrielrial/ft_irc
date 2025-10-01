#!/bin/bash
{
    echo -e "PASS 42"
    echo -e "USER netcatbottom 0 * :netcatbottom"
    echo -e "NICK ncb"
    cat
} | nc localhost 6667
