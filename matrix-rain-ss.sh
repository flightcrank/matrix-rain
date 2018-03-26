#!/bin/bash

#This script is dependant on 'xprintidle' program installed on the system
#its purpose is to be run as a cronjob on a linux system to act as a screensaver
#the $DIR variable has to be set to the path the executeable is located
#the $MINS variable sets the number of minutes the script should wait to run 
#the exectueable while the system is idle

export DISPLAY=:0.0

#get the amount of milliseconds the system has been idle for
IDLE=$(xprintidle)

#the path where the executable is located
DIR=~/Code/c/matrix-rain

#the number of minutes the system is idle for before executing the if statement
MINS=5

#converting minutes to milliseconds
let TIME=$MINS*60*1000

if [ $IDLE -gt $TIME ]
then
	cd $DIR
	./matrix-rain-2 -f
else
	echo only $IDLE milliseconds have passed which is less than $TIME
fi

