#!/usr/bin/env bash

for f in tc/*; do
   echo "$f"
   build/exec < $f
done