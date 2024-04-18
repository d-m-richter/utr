#!/usr/bin/env python3
from pathlib import Path
import random
import subprocess
import numpy as np
import os

hollow_cylinder_default = 1 #mm

#execute_detectorvariation = "./Detector_Variation.py"
#subprocess.run(execute_detectorvariation, shell=True)
#execute_distancevariation = "./Distance_Variation.py"
#subprocess.run(execute_distancevariation, shell=True)
#execute_activityvariation = "./Activity_Variation.py"
#subprocess.run(execute_activityvariation, shell=True)

# search for path to files that have place holders in them and that have to be changed into

DetectorConstruction_old_file_path = Path().resolve()/Path('Templates/GEM_Series_HPGe_GEM20P470_Template.cc')
DetectorConstruction_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/DetectorConstruction.cc')

utrwrapper_efficiency_jonny_old_file_path = Path().resolve()/Path('Templates/utrwrapper-efficiency-jonny_Template.xmac')
utrwrapper_efficiency_jonny_new_file_path = Path().resolve()/Path('macros/examples/utrwrapper-efficiency-jonny.xmac')

Hohlzylinder_old_file_path = Path().resolve()/Path('Templates/Hohlzylinder_Template.gdml')
Hohlzylinder_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/Volumes/Hohlzylinder.gdml')

Quellenhalter_old_file_path = Path().resolve()/Path('Templates/Quellenhalter_Template.gdml')
Quellenhalter_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/Volumes/Quellenhalter.gdml')

# open and read in the old files that have place holders in them

with open(utrwrapper_efficiency_jonny_old_file_path) as utrwrapper_efficiency_jonny_old_file: # --------- activity
        utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_file.read()

with open(DetectorConstruction_old_file_path) as DetectorConstruction_old_file: # --------- detector construction
            DetectorConstruction_old_content = DetectorConstruction_old_file.read()

with open(Hohlzylinder_old_file_path) as Hohlzylinder_old_file: # --------- Hohlzylinder
        Hohlzylinder_old_content = Hohlzylinder_old_file.read()

with open(Quellenhalter_old_file_path) as Quellenhalter_old_file: # --------- Quellenhalter
            Quellenhalter_old_content = Quellenhalter_old_file.read()

# define functions to change the content (place holders) of the old files

def utrwrapper_efficiency_jonny(disintegration_old, disintegration_new, folder_old, folder_new, horizontal_shift_x_old, horizontal_shift_y_new, horizontal_shift_y_old, horizontal_shift_x_new, utrwrapper_efficiency_jonny_old_content): 
    # from Activity_Variation.py
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(disintegration_old, disintegration_new) # --- activity: change the numbers of particles that are produced in a simulation run
    
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(folder_old, folder_new) # ---- folder: change the name of the folder in which the efficiency data is stored 
    
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(horizontal_shift_x_old, horizontal_shift_x_new)
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(horizontal_shift_y_old, horizontal_shift_y_new)

    with open(utrwrapper_efficiency_jonny_new_file_path, 'w') as utrwrapper_efficiency_jonny_new_file: # ------------ open new files 
        utrwrapper_efficiency_jonny_new_file.write(utrwrapper_efficiency_jonny_old_content) # ---------- change the content of the new files that go into the simulation run

def DetectorConstruction(crystal_diameter_old, crystal_diameter_new, crystal_length_old, crystal_length_new, hole_diameter_old, hole_diameter_new, hole_depth_old, hole_depth_new, dead_layer_side_old, dead_layer_side_new, dead_layer_top_old, dead_layer_top_new, source_thickness_old, source_thickness_new, end_cap_length_above_table_old, end_cap_length_above_table_new, hollow_cylinder_old, hollow_cylinder_new, hollow_cylinder_position_old, hollow_cylinder_position_new, distance_source_detector_old, distance_source_detector_new, horizontal_shift_x_old, horizontal_shift_x_new, horizontal_shift_y_old, horizontal_shift_y_new, DetectorConstruction_old_content):
        # from Detector_Variation.py
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(crystal_diameter_old, crystal_diameter_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(crystal_length_old, crystal_length_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hole_diameter_old, hole_diameter_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hole_depth_old, hole_depth_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(dead_layer_side_old, dead_layer_side_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(dead_layer_top_old, dead_layer_top_new)

        # from Distance_Variation.py
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(source_thickness_old, source_thickness_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(end_cap_length_above_table_old, end_cap_length_above_table_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hollow_cylinder_old, hollow_cylinder_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hollow_cylinder_position_old, hollow_cylinder_position_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(distance_source_detector_old, distance_source_detector_new)

        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(horizontal_shift_x_old, horizontal_shift_x_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(horizontal_shift_y_old, horizontal_shift_y_new)


        with open(DetectorConstruction_new_file_path, 'w') as DetectorConstruction_new_file:
            DetectorConstruction_new_file.write(DetectorConstruction_old_content)

def Hohlzylinder_change(hollow_cylinder_old, hollow_cylinder_new, Hohlzylinder_old_content):
        
        Hohlzylinder_old_content = Hohlzylinder_old_content.replace(hollow_cylinder_old, hollow_cylinder_new)
        
        with open(Hohlzylinder_new_file_path, 'w') as Hohlzylinder_new_file:
            Hohlzylinder_new_file.write(Hohlzylinder_old_content)

def Quellenhalter_change(source_holder_old, source_holder_new, notch_old, notch_new, sh1_old, sh1_new, sh2_old, sh2_new, sh3_old, sh3_new, sh4_old, sh4_new, sh5_old, sh5_new, sh6_old, sh6_new, sh7_old, sh7_new, Quellenhalter_old_content):
        
        Quellenhalter_old_content = Quellenhalter_old_content.replace(source_holder_old, source_holder_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(notch_old, notch_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh1_old, sh1_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh2_old, sh2_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh3_old, sh3_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh4_old, sh4_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh5_old, sh5_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh6_old, sh6_new)
        Quellenhalter_old_content = Quellenhalter_old_content.replace(sh7_old, sh7_new)
        
        with open(Quellenhalter_new_file_path, 'w') as Quellenhalter_new_file:
            Quellenhalter_new_file.write(Quellenhalter_old_content)

# for going through all detectorvariation_i.txt and distancevariation_j.txt individualy

execute_utrwrapper = "./OutputProcessing/utrwrapper.py macros/examples/utrwrapper-efficiency-jonny.xmac" # ---------- command for running utr
    
for i in range(0,4):
    for j in range(0,4):
        for l in range(0,40):
            # read in detectorvariation_i.txt and distancevariation_j.txt files --- for every i and j
            detectorvariation_file_path = Path().resolve()/Path(f'detectorvariation_{i}.txt')
            detectorvariation_data = detectorvariation_file_path
            distancevariation_file_path = Path().resolve()/Path(f'distancevariation_{j}.txt')#(f'output/GEM_Variation/Efficiency_{hollow_cylinder_default}mm/distancevariation_{j}.txt')
            distancevariation_data = distancevariation_file_path
            # read in activity.txt file --- only ones
            activity_file_path = Path(f'activity.txt')#(f'output/GEM_Variation/Efficiency_{hollow_cylinder_default}mm/activity.txt')
            activity_data = activity_file_path.resolve()
    
            # read in values from detectorconstruction_{i}{j} for each column to parameters
            crystal_diameter = np.loadtxt(detectorvariation_data, skiprows=1, usecols=0)
            crystal_length = np.loadtxt(detectorvariation_data, skiprows=1, usecols=1)
            hole_diameter = np.loadtxt(detectorvariation_data, skiprows=1, usecols=2)
            hole_depth = np.loadtxt(detectorvariation_data, skiprows=1, usecols=3)
            dead_layer_side = np.loadtxt(detectorvariation_data, skiprows=1, usecols=4)
            dead_layer_top = np.loadtxt(detectorvariation_data, skiprows=1, usecols=5)
            
            hollow_cylinder = np.loadtxt(distancevariation_data, skiprows=1, usecols=0)
            source_holder = np.loadtxt(distancevariation_data, skiprows=1, usecols=1)
            notch = np.loadtxt(distancevariation_data, skiprows=1, usecols=2)
            source_thickness = np.loadtxt(distancevariation_data, skiprows=1, usecols=3)
            end_cap_length_above_table = np.loadtxt(distancevariation_data, skiprows=1, usecols=4)
            distance_source_detector = np.loadtxt(distancevariation_data, skiprows=1, usecols=5)

            horizontal_shift_x = np.loadtxt(distancevariation_data, skiprows=1, usecols=6)
            horizontal_shift_y = np.loadtxt(distancevariation_data, skiprows=1, usecols=7)
      
            # read in values from activity from second column to parameters 
            disintegrations = np.loadtxt(activity_data, skiprows=1, usecols=1) 

            #k = random.randint(0, 39)

            # change place holders 
            # declarate place holder in utrwrapper-efficiency-jonny.xmac that has only to be changed in the k loop
            disintegration_old = "{{disintegrations}}"
            disintegration_new = str(int(disintegrations[l]))

            #hollow_cylinder_position = distance_source_detector[l] + "HPGe_GEM20P470_props.end_cap_length" - end_cap_length_above_table + hollow_cylinder * 0.5 #hollow_cylinder[l] + source_holder[l] - notch[l] + source_thickness[l]/2 - end_cap_length_above_table[l]
            
            notch_abs = source_holder[l] - notch[l]
            sh1 = source_holder[l] - 2.5
            sh2 = source_holder[l] - 3.5
            sh3 = source_holder[l] - 4.5
            sh4 = source_holder[l] - 5.5
            sh5 = source_holder[l] - 6.5
            sh6 = source_holder[l] - 7.5
            sh7 = source_holder[l] - 8.5

            # declarate place holder in DetectorConstruction.cc that has also be changed in the l loop
            crystal_diameter_old = "{{crystal_diameter}}"
            crystal_diameter_new = str(crystal_diameter[l])
            crystal_length_old = "{{crystal_length}}"
            crystal_length_new = str(crystal_length[l])
            hole_diameter_old = "{{hole_diameter}}"
            hole_diameter_new = str(hole_diameter[l])
            hole_depth_old = "{{hole_depth}}"
            hole_depth_new = str(hole_depth[l])
            dead_layer_side_old = "{{dead_layer_side}}"
            dead_layer_side_new = str(dead_layer_side[l])
            dead_layer_top_old = "{{dead_layer_top}}"
            dead_layer_top_new = str(dead_layer_top[l])
                
            source_thickness_old = "{{source_thickness}}"
            source_thickness_new = str(source_thickness[l])
            end_cap_length_above_table_old = "{{end_cap_length_above_table}}"
            end_cap_length_above_table_new = str(end_cap_length_above_table[l])

            hollow_cylinder_position_old = "{{hollow_cylinder_position}}"
            hollow_cylinder_position_new = "distance_source_detector + " + "134 * mm - (134 * mm - " + str(end_cap_length_above_table[l]) + ")" #str(hollow_cylinder_position)

            # declarate place holder in utrwrapper-efficiency-jonny.xmac that has also be changed in the j loop
            folder_old = "{{folder}}"
            folder_new = "GEM_Variation/Efficiency_" + str(hollow_cylinder_default) + "mm/Efficiency_" + str(i) + str(j) + "/Efficiency_" + str(i) + "_" + str(j) + "_" + str(l) #+ str(k) + "_" + str(l)

            # declarate place holder in Hohlzylinder_new.gdml that has also be changed in the j loop
            hollow_cylinder_old = "{{hollow_cylinder}}"
            hollow_cylinder_new = str(hollow_cylinder[l])

            # declarate place holder in Quellenhalter_new.gdml that has also be changed in the j loop
            source_holder_old = "{{source_holder}}"
            source_holder_new = str(source_holder[l])
            notch_old = "{{notch}}"
            notch_new = str(notch_abs)
            sh1_old = "{{141.53}}"
            sh1_new = str(sh1)
            sh2_old = "{{140.53}}"
            sh2_new = str(sh2)
            sh3_old = "{{139.53}}"
            sh3_new = str(sh3)
            sh4_old = "{{138.53}}"
            sh4_new = str(sh4)
            sh5_old = "{{137.53}}"
            sh5_new = str(sh5)
            sh6_old = "{{136.53}}"
            sh6_new = str(sh6)
            sh7_old = "{{135.53}}"
            sh7_new = str(sh7)

            horizontal_shift_x_old = "{{horizontal_shift_x}}"
            horizontal_shift_x_new = str(horizontal_shift_x[l])
            horizontal_shift_y_old = "{{horizontal_shift_y}}"
            horizontal_shift_y_new = str(horizontal_shift_y[l])

            distance_source_detector_old = "{{distance_source_detector}}"
            distance_source_detector_new = str(distance_source_detector[l])

            utrwrapper_efficiency_jonny(disintegration_old, disintegration_new, folder_old, folder_new, horizontal_shift_x_old, horizontal_shift_y_new, horizontal_shift_y_old, horizontal_shift_x_new, utrwrapper_efficiency_jonny_old_content) # ------------- change values in utrwrapper-efficiency-jonny.xmac
            DetectorConstruction(crystal_diameter_old, crystal_diameter_new, crystal_length_old, crystal_length_new, hole_diameter_old, hole_diameter_new, hole_depth_old, hole_depth_new, dead_layer_side_old, dead_layer_side_new, dead_layer_top_old, dead_layer_top_new, source_thickness_old, source_thickness_new, end_cap_length_above_table_old, end_cap_length_above_table_new, hollow_cylinder_old, hollow_cylinder_new, hollow_cylinder_position_old, hollow_cylinder_position_new, distance_source_detector_old, distance_source_detector_new, horizontal_shift_x_old, horizontal_shift_x_new, horizontal_shift_y_old, horizontal_shift_y_new, DetectorConstruction_old_content) # ---- change values in DetectorConstruction.cc
            Hohlzylinder_change(hollow_cylinder_old, hollow_cylinder_new, Hohlzylinder_old_content) # ---------- change values in Hohlzylinder_new.gdml
            Quellenhalter_change(source_holder_old, source_holder_new, notch_old, notch_new, sh1_old, sh1_new, sh2_old, sh2_new, sh3_old, sh3_new, sh4_old, sh4_new, sh5_old, sh5_new, sh6_old, sh6_new, sh7_old, sh7_new, Quellenhalter_old_content) # ---------- change values in Quellenhalter_new.gdml
            subprocess.run(execute_utrwrapper, shell=True)# ---------- run utr

# execute the calculation file for the efficiency, which creates a txt. file with all 40 efficiencies with the corresponding disintegrations
#execute_efficiencycalculation = "./output/Efficiency_Calculation_Simulation.py"
#subprocess.run(execute_efficiencycalculation, shell=True)
    
        



