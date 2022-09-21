#!/bin/bash

for sim in /home/mbeuschlein/utr/macros/angular_efficiency_112Sn_wrongDelta/angdist*.xmac; do
	echo 'Simulating ${sim}'
	OutputProcessing/utrwrapper.py ${sim}

done
