#!/bin/bash

gcc -lnsl -lresolv udpServerHash.c -o udpServerHash -lm && \
  ./udpServerHash 127.0.0.1 12001 64 255 127.0.0.1 12004 128 127.0.0.1 12002
