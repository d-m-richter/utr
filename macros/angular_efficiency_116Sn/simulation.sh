#!/bin/bash

for sim in /home/mbeuschlein/utr/macros/angular_efficieny_116Sn/*.xmac; do
	echo 'Simulating ${sim}'
	OutputProcessing/utrwrapper.py ${sim}

done
