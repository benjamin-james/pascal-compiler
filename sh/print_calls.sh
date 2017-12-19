#!/usr/bin/env bash
./callstack.sh | gdb --args $@ | grep '#0' | tr -s ' ' | cut -d ' ' -f 2
