#!/bin/bash

gcc -lnsl -lresolv udpClientHash.c -o udpClientHash -lrt && ./udpClientHash localhost 12001
