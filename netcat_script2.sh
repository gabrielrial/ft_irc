#!/bin/bash
{
    echo -e "PASS 42"
    echo -e "USER nc0!@:%m cluster_host_name () :netcatbottom"
    echo -e "NICK ncb"
    cat
} | nc localhost 6667
