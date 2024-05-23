#!/bin/sh

# deve ser executado dentro da pasta do projeto
# ex: /home/pi/pico/projetos/blink_PicoW

# dependo de como o sdk foi instalado, a linha do export nao é necessaria
# mas deve ser ajustada para o path do sdk
# por exemplo, /home/pi/pico/pico-sdk
# export PICO_SDK_PATH=../../pico-sdk

mkdir build
cd build
cmake ..
make