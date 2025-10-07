#!/bin/bash
{
    echo -e "PASS 42"
    echo -e "USER canbeanything * 0 :netcattop"
    echo -e "NICK nct"
    cat
} | nc localhost 6667
