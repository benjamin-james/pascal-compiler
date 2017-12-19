#!/usr/bin/env bash
ls src/parser/prod/*.c | xargs -n1 basename | parallel 'echo break {.}'

echo "commands 1-$(ls src/parser/prod/*.c | wc -l)"
echo silent
echo bt 
echo c
echo end
echo r
echo quit
