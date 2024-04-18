#!/usr/bin/env python3
from pathlib import Path
import random
import subprocess
import numpy as np
import os

# search for path to files that have place holders in them and that have to be changed into

PlasticBottle_old_file_path = Path().resolve()/Path('Templates/Bottels/PlasticBottle_Template.hh')
PlasticBottle_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/PlasticBottle.hh')

utrwrapper_efficiency_jonny_old_file_path = Path().resolve()/Path('Templates/Bottels/PETbottle-238U.xmac')
utrwrapper_efficiency_jonny_new_file_path = Path().resolve()/Path('macros/examples/utrwrapper-efficiency-jonny.xmac')

# open and read in the old files that have place holders in them

with open(PlasticBottle_old_file_path) as PlasticBottle_old_file: # --------- detector construction
            PlasticBottle_old_content = PlasticBottle_old_file.read()

with open(utrwrapper_efficiency_jonny_old_file_path) as utrwrapper_efficiency_jonny_old_file: # --------- activity
        utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_file.read()

# define functions to change the content (place holders) of the old files

def PlasticBottle(density_old, density_new, PlasticBottle_old_content):

        PlasticBottle_old_content = PlasticBottle_old_content.replace(density_old, density_new)


        with open(PlasticBottle_new_file_path, 'w') as PlasticBottle_new_file:
            PlasticBottle_new_file.write(PlasticBottle_old_content)

def utrwrapper_efficiency_jonny(folder_old, folder_new, utrwrapper_efficiency_jonny_old_content): 

    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(folder_old, folder_new) # ---- folder: change the name of the folder in which the efficiency data is stored 

    with open(utrwrapper_efficiency_jonny_new_file_path, 'w') as utrwrapper_efficiency_jonny_new_file: # ------------ open new files 
        utrwrapper_efficiency_jonny_new_file.write(utrwrapper_efficiency_jonny_old_content) # ---------- change the content of the new files that go into the simulation run


# for going through all detectorvariation_i.txt and distancevariation_j.txt individualy

execute_utrwrapper = "./OutputProcessing/utrwrapper.py macros/examples/utrwrapper-efficiency-jonny.xmac" # ---------- command for running utr
    
for i in range(0,10):

    if i == 0:
        density = 3.97
    elif i == 1:
        density = 0.839
    elif i > 1:
        density = random.uniform(0.5, 4.5)

    density_old = "{{density}}"
    density_new = str(density)

    folder_old = "{{folder}}"
    folder_new = "Density_Test/Efficiency_" + str(density)

    utrwrapper_efficiency_jonny(folder_old, folder_new, utrwrapper_efficiency_jonny_old_content) # ------------- change values in utrwrapper-efficiency-jonny.xmac
    PlasticBottle(density_old, density_new, PlasticBottle_old_content) # ---- change values in DetectorConstruction.cc
    subprocess.run(execute_utrwrapper, shell=True)# ---------- run utr
    
        



