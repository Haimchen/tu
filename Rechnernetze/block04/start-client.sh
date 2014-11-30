#!/bin/bash

gcc -lnsl -lresolv udpClientHash.c -o udpClientHash && ./udpClientHash localhost 12001
