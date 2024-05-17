Passos para instalação:

sudo apt update
sudo apt upgrade

sudo apt install -y git gcc g++ clang python3 mosquitto mosquitto-clients redis-server redis-tools postgresql apt-transport-https dirmngr

sudo apt autoremove

mkdir ~/lorawan
cd ~/lorawan

git clone https://github.com/Elecrow-RD/LR1302_loraWAN.git

cd LR1302_loraWAN

mv ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/ ~/lorawan/

~/lorawan/

rm -rf LR1302_loraWAN

cd ~/lorawan/cd sx1302_hal

make


