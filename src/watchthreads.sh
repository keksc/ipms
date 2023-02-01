#!/bin/bash
watch ps -o thcount `ps -A | grep ipms.out | mawk '{ print $1 }'`