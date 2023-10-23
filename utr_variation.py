from pathlib import Path
import subprocess
import numpy as np

# search for path to files that have place holders in them and that have to be changed into

DetectorConstruction_old_file_path = Path().resolve()/Path('Changing_files/DetectorConstruction_old.cc')
DetectorConstruction_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/DetectorConstruction.cc')

utrwrapper_efficiency_jonny_old_file_path = Path().resolve()/Path('Changing_files/utrwrapper-efficiency-jonny_old.xmac')
utrwrapper_efficiency_jonny_new_file_path = Path().resolve()/Path('macros/examples/utrwrapper-efficiency-jonny.xmac')

Hohlzylinder_old_file_path = Path().resolve()/Path('Changing_files/Hohlzylinder_old.gdml')
Hohlzylinder_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/Volumes/Hohlzylinder_new.gdml')

Quellenhalter_old_file_path = Path().resolve()/Path('Changing_files/Quellenhalter_old.gdml')
Quellenhalter_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/Volumes/Quellenhalter_new.gdml')

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

def activity_change(activity_old, activity_new, source_placement_old, source_placement_new, utrwrapper_efficiency_jonny_old_content): 
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(activity_old, activity_new) # --- activity: change the numbers of particles that are produced in a simulation run
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(folder_old, folder_new) # ---- folder: change the name of the folder in which the efficiency data is stored 
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(source_placement_old, source_placement_new) # ---- source placement: change the placement of the source according to the new distance between source and crystal 
    with open(utrwrapper_efficiency_jonny_new_file_path, 'w') as utrwrapper_efficiency_jonny_new_file: # ------------ open new files 
        utrwrapper_efficiency_jonny_new_file.write(utrwrapper_efficiency_jonny_old_content) # ---------- change the content of the new files that go into the simulation run

def detectorconstruction_change(crystal_diameter_old, crystal_diameter_new, crystal_length_old, crystal_length_new, hole_diameter_old, hole_diameter_new, hole_depth_old, hole_depth_new, dead_layer_side_old, dead_layer_side_new, dead_layer_top_old, dead_layer_top_new, distance_detector_end_cap_old, distance_detector_end_cap_new, source_thickness_old, source_thickness_new, end_cap_face_thickness_old, end_cap_face_thickness_new, end_cap_stand_out_old, end_cap_stand_out_new, source_placement_old, source_placement_new, hollow_cylinder_old, hollow_cylinder_new, DetectorConstruction_old_content):
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(crystal_diameter_old, crystal_diameter_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(crystal_length_old, crystal_length_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hole_diameter_old, hole_diameter_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hole_depth_old, hole_depth_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(dead_layer_side_old, dead_layer_side_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(dead_layer_top_old, dead_layer_top_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(distance_detector_end_cap_old, distance_detector_end_cap_new)
        
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(source_thickness_old, source_thickness_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(end_cap_face_thickness_old, end_cap_face_thickness_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(end_cap_stand_out_old, end_cap_stand_out_new)

        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(source_placement_old, source_placement_new) # ---- source placement: change the placement of the plastikzylinder
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hollow_cylinder_old, hollow_cylinder_new)
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
       

command = "./OutputProcessing/utrwrapper.py macros/examples/utrwrapper-efficiency-jonny.xmac" # ---------- command for running utr

for i in range(4):
    for j in range(4):
        # read in detectordimension_ij.txt files --- for every i and j
        detectordimension_file_path = Path().resolve()/Path(f'DetectorConstruction/Others/Efficiency/detectordimension_{i}{j}.txt')
        detectordimension_data = detectordimension_file_path
        # read in activity.txt file --- only ones
        activity_file_path = Path('macros/examples/activity.txt')
        activity_data = activity_file_path.resolve()

        # read in values from detectorconstruction_{i}{j} for each column to parameters
        crystal_diameter = np.loadtxt(detectordimension_data, skiprows=39, usecols=0)
        crystal_length = np.loadtxt(detectordimension_data, skiprows=39, usecols=1)
        active_volume = np.loadtxt(detectordimension_data, skiprows=39, usecols=2)
        hole_diameter = np.loadtxt(detectordimension_data, skiprows=39, usecols=3)
        hole_depth = np.loadtxt(detectordimension_data, skiprows=39, usecols=4)
        dead_layer_side = np.loadtxt(detectordimension_data, skiprows=39, usecols=5)
        dead_layer_top = np.loadtxt(detectordimension_data, skiprows=39, usecols=6)
        distance_detector_end_cap = np.loadtxt(detectordimension_data, skiprows=39, usecols=7)
        hollow_cylinder = np.loadtxt(detectordimension_data, skiprows=39, usecols=8)
        source_holder = np.loadtxt(detectordimension_data, skiprows=39, usecols=9)
        notch = np.loadtxt(detectordimension_data, skiprows=39, usecols=10)
        source_thickness = np.loadtxt(detectordimension_data, skiprows=39, usecols=11)
        end_cap_face_thickness = np.loadtxt(detectordimension_data, skiprows=39, usecols=12)
        end_cap_stand_out = np.loadtxt(detectordimension_data, skiprows=39, usecols=13)
        distance_source_detector = np.loadtxt(detectordimension_data, skiprows=39, usecols=14)

        #print(crystal_diameter) 

        # read in values from activity from second column to parameters 
        disintegrations = np.loadtxt(activity_data, skiprows=39, usecols=1) 

        #print(disintegrations) 
    
        # change place holders 
        for k in range(2):#len(disintegrations)):
            # declarate place holder in utrwrapper-efficiency-jonny.xmac that has only to be changed in the k loop
            activity_old = "{{disintegrations}}"
            activity_new = str(int(disintegrations[k]))
        
            for l in range(2):#len(crystal_diameter)):
                source_placement = distance_source_detector[l] - 100. # ----- calculate the placement of the radioactive source in the simulation 
                notch_abs = source_holder[l] - notch[l]
                sh1 = source_holder[l] - 2.5
                sh2 = source_holder[l] - 3.5
                sh3 = source_holder[l] - 4.5
                sh4 = source_holder[l] - 5.5
                sh5 = source_holder[l] - 6.5
                sh6 = source_holder[l] - 7.5
                sh7 = source_holder[l] - 8.5

                # declarate place holder in DetectorConstruction.cc that has also be changed in the l loop
                crystal_diameter_old = "{{cystal_diameter}}"
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
            
                distance_detector_end_cap_old = "{{distance_detector_end_cap}}"
                distance_detector_end_cap_new = str(distance_detector_end_cap[l])
                source_thickness_old = "{{source_thickness}}"
                source_thickness_new = str(source_thickness[l])
                end_cap_face_thickness_old = "{{end_cap_face_thickness}}"
                end_cap_face_thickness_new = str(end_cap_face_thickness[l])
                end_cap_stand_out_old = "{{end_cap_stand_out}}"
                end_cap_stand_out_new = str(end_cap_stand_out[l])

                # declarate place holder in utrwrapper-efficiency-jonny.xmac that has also be changed in the j loop
                folder_old = "{{folder}}"
                folder_new = "Efficiency_" + str(i) + str(j) + "_" + str(k) + str(l)
                source_placement_old = "{{source_placement}}"
                source_placement_new = str(source_placement)

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

                activity_change(activity_old, activity_new, source_placement_old, source_placement_new, utrwrapper_efficiency_jonny_old_content) # ------------- change values in utrwrapper-efficiency-jonny.xmac
                detectorconstruction_change(crystal_diameter_old, crystal_diameter_new, crystal_length_old, crystal_length_new, hole_diameter_old, hole_diameter_new, hole_depth_old, hole_depth_new, dead_layer_side_old, dead_layer_side_new, dead_layer_top_old, dead_layer_top_new, distance_detector_end_cap_old, distance_detector_end_cap_new, source_thickness_old, source_thickness_new, end_cap_face_thickness_old, end_cap_face_thickness_new, end_cap_stand_out_old, end_cap_stand_out_new, source_placement_old, source_placement_new, hollow_cylinder_old, hollow_cylinder_new, DetectorConstruction_old_content) # ---- change values in DetectorConstruction.cc
                Hohlzylinder_change(hollow_cylinder_old, hollow_cylinder_new, Hohlzylinder_old_content) # ---------- change values in Hohlzylinder_new.gdml
                Quellenhalter_change(source_holder_old, source_holder_new, notch_old, notch_new, sh1_old, sh1_new, sh2_old, sh2_new, sh3_old, sh3_new, sh4_old, sh4_new, sh5_old, sh5_new, sh6_old, sh6_new, sh7_old, sh7_new, Quellenhalter_old_content) # ---------- change values in Quellenhalter_new.gdml
                subprocess.run(command, shell=True)# ---------- run utr
       
    
        



