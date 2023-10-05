#!/bin/bash

# irc-client.sh
#
# Author: Noah BEAUFILS
# Date: 4-oct-2023

binName=ft_irc

if [ $# -eq 2 ]; then
	host=$1
	port=$2

	clear
	echo "Using telnet" $host $port
	echo "[ ... ]"

	telnet $host $port
else
	echo "Usage:" $0 "<host> <port>"
fi