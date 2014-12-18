#!/bin/bash

gcc -lnsl -lresolv udpServerHash.c -o udpServerHash -lm && \
  ./udpServerHash 127.0.0.1 12003 192 128 127.0.0.1 12002 255 127.0.0.1 12004
