#!/bin/bash

LENGTH=$((4*1024-16))
dd if=$1 skip=16 bs=1 count=$LENGTH | openssl md5 -binary > $2
dd if=$1 skip=16 bs=1 >>  $2

