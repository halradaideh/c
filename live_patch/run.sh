#!/bin/bash
set -e

clear

echo "Looking for src System Map file based on installed kernel version .."
sleep 1
echo "Installed Kernel version $(uname -r)"
sleep 1
echo "file to look in /boot/System.map-$(uname -r)"
sleep 1
echo "searching for table address .."
sleep 1

c028863cvi

sys_call_table_address=$(cat /boot/System.map-$(uname -r) | grep sys_call | awk '{print $1}')
echo "System Call Table Address is : ${sys_call_table_address}"
sleep 5

clear
echo "Building exporter"
echo
cd sys_export
sed -i "s/TO-BE-Filled/${sys_call_table_address}/g" ./sys_export.c
make
echo
echo "Build successful"
sleep 5
clear
echo "Installing exporter LKM into the Kernel .."
echo
insmod sys_export.ko
echo
dmesg | tail -n 3
echo
echo "Build success"
sleep 5
clear
echo "Building new system call .."
echo
cd ../newcall
make
echo
echo "Build success"
sleep 5
clear
echo "Installing new system call replacement LKM into the Kernel .."
echo
insmod newcall.ko
echo
dmesg | tail -n 3
echo
echo "Installing success"
sleep 5
clear

echo "Testing .."
cd ../test
g++ test.cpp -o test

echo

./test

