# SpaIot Simulator

This project is a simulator for the SpaIot project. 
It generates the spa signals and simulates the spa environment.
Runs on all pi boards supported by piduino.

## Installation

Install piduino on your raspberry pi, then clone the project and compile it, then install it.

```bash
wget -O- http://www.piduino.org/piduino-key.asc | sudo gpg --dearmor --yes --output /usr/share/keyrings/piduino-archive-keyring.gpg
echo "deb [signed-by=/usr/share/keyrings/piduino-archive-keyring.gpg] http://raspbian.piduino.org buster piduino" | sudo tee /etc/apt/sources.list.d/piduino.list
sudo apt update
sudo apt install libpiduino-dev piduino-utils libcppdb-dev pkg-config cmake libudev-dev libncurses-dev
git clone https://github.com/epsilonrt/spaiot-simulator.git
cd spaiot-simulator
mkdir cmake-build-Release
cd cmake-build-Release
cmake ..
make 
sudo make install
```

## Usage

`spaiot-simulator dataOutPin clkPin nWrPin dataInPin`, eg: 

```bash
spaiot-simulator 16 15 1 4
```
