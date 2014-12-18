#!/bin/bash

gcc -lnsl -lresolv udpServerHash.c -o udpServerHash -lm	 && \
  ./udpServerHash 127.0.0.1 12002 128 64 127.0.0.1 12001 192 127.0.0.1 12003
