#!/bin/bash
export DISPLAY=:0
Xvfb :0 -screen 0 1024x768x24 &
python3 inmet.py >> teste.txt
cat teste.txt
