#!/bin/bash

gcc -lnsl -lresolv udpServerHash.c -o udpServerHash -lm && \
  ./udpServerHash 127.0.0.1 12004 255 192 127.0.0.1 12003 64 127.0.0.1 12001
