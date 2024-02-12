#!/bin/bash

# irc-client.sh
#
# Author: Noah BEAUFILS
# Date: 9-oct-2023

binName=ft_irc

if [ $# -ge 1 ]; then
	if [ $# -eq 1 ]; then
		host="localhost"
		port=$1
	else
		host=$1
		port=$2
	fi

	clear
	echo -e "    \e[36m\e[1m[ ==== IRC Client ==== ]\e[0m\n"
	echo "Connecting with nc" $host $port "..."

	nc $host $port
	res=$?
	if [ $res -eq 1 ]; then
		echo "error: [" $res "] the connection failed"
	fi
else
	echo "Usage:" $0 "<host> <port>"
fi