#!/bin/bash

gcc -lnsl -lresolv udpServerHash.c -o udpServerHash && \
  ./udpServerHash 127.0.0.1 12002 128 255 127.0.0.1 12001 255 127.0.0.1 12001
