#!/bin/bash

for sim in /home/mbeuschlein/utr/macros/angular_efficiency_112Sn/*.xmac; do
	echo 'Simulating ${sim}'
	OutputProcessing/utrwrapper.py ${sim}

done
