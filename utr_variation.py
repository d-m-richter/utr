from pathlib import Path
import subprocess
import numpy as np

# search for path to .txt files

activity_file_path = Path('macros/examples/activity.txt')
activity_data = activity_file_path.resolve()

DetectorConstruction_old_file_path = Path().resolve()/Path('Changing_files/DetectorConstruction_old.cc')
DetectorConstruction_new_file_path = Path().resolve()/Path('DetectorConstruction/Others/Efficiency/DetectorConstruction.cc')

utrwrapper_efficiency_jonny_old_file_path = Path().resolve()/Path('Changing_files/utrwrapper-efficiency-jonny_old.xmac')
utrwrapper_efficiency_jonny_new_file_path = Path().resolve()/Path('macros/examples/utrwrapper-efficiency-jonny.xmac')

# read in parameters to change
disintegrations = np.loadtxt(activity_data, skiprows=40, usecols=1) 



# open  and read in .cc an .mac files
# activity
with open(utrwrapper_efficiency_jonny_old_file_path) as utrwrapper_efficiency_jonny_old_file:
        utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_file.read()
# detector construction
with open(DetectorConstruction_old_file_path) as DetectorConstruction_old_file:
            DetectorConstruction_old_content = DetectorConstruction_old_file.read()

# define functions to change content of .cc and .mac files

def activity_change(activity_old, activity_new, utrwrapper_efficiency_jonny_old_content):
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(activity_old, activity_new)
    utrwrapper_efficiency_jonny_old_content = utrwrapper_efficiency_jonny_old_content.replace(folder_old, folder_new)
    with open(utrwrapper_efficiency_jonny_new_file_path, 'w') as utrwrapper_efficiency_jonny_new_file:
        utrwrapper_efficiency_jonny_new_file.write(utrwrapper_efficiency_jonny_old_content)
        #print(utrwrapper_efficiency_jonny_new_file)
        #print(utrwrapper_efficiency_jonny_old_content)

def detectorconstruction_change(crystal_diameter_old, crystal_diameter_new, crystal_length_old, crystal_length_new, hole_diameter_old, hole_diameter_new, hole_depth_old, hole_depth_new, dead_layer_side_old, dead_layer_side_new, dead_layer_top_old, dead_layer_top_new, distance_detector_end_cap_old, distance_detector_end_cap_new, hollow_cylinder_old, hollow_cylinder_new, source_holder_old, source_holder_new, notch_old, notch_new, source_thickness_old, source_thickness_new, end_cap_face_thickness_old, end_cap_face_thickness_new, end_cap_stand_out_old, end_cap_stand_out_new, DetectorConstruction_old_content):
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(crystal_diameter_old, crystal_diameter_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(crystal_length_old, crystal_length_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hole_diameter_old, hole_diameter_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hole_depth_old, hole_depth_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(dead_layer_side_old, dead_layer_side_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(dead_layer_top_old, dead_layer_top_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(distance_detector_end_cap_old, distance_detector_end_cap_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(hollow_cylinder_old, hollow_cylinder_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(source_holder_old, source_holder_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(notch_old, notch_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(source_thickness_old, source_thickness_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(end_cap_face_thickness_old, end_cap_face_thickness_new)
        DetectorConstruction_old_content = DetectorConstruction_old_content.replace(end_cap_stand_out_old, end_cap_stand_out_new)
        with open(DetectorConstruction_new_file_path, 'w') as DetectorConstruction_new_file:
            DetectorConstruction_new_file.write(DetectorConstruction_old_content)
            #print(DetectorConstruction_new_file)
            #print(DetectorConstruction_old_content)

command = "./OutputProcessing/utrwrapper.py macros/examples/utrwrapper-efficiency-jonny.xmac"

for k in range(4):
    # einlesen der detectordimension_[k].txt Files
    detectordimension_file_path = Path().resolve()/Path(f'DetectorConstruction/Others/Efficiency/detectordimension_{k}.txt')
    detectordimension_data = detectordimension_file_path
    # Parameterdeklarierung in arrays
    crystal_diameter = np.loadtxt(detectordimension_data, skiprows=38, usecols=0)
    crystal_length = np.loadtxt(detectordimension_data, skiprows=38, usecols=1)
    active_volume = np.loadtxt(detectordimension_data, skiprows=38, usecols=2)
    hole_diameter = np.loadtxt(detectordimension_data, skiprows=38, usecols=3)
    hole_depth = np.loadtxt(detectordimension_data, skiprows=38, usecols=4)
    dead_layer_side = np.loadtxt(detectordimension_data, skiprows=38, usecols=5)
    dead_layer_top = np.loadtxt(detectordimension_data, skiprows=38, usecols=6)
    distance_detector_end_cap = np.loadtxt(detectordimension_data, skiprows=38, usecols=7)
    hollow_cylinder = np.loadtxt(detectordimension_data, skiprows=38, usecols=8)
    source_holder = np.loadtxt(detectordimension_data, skiprows=38, usecols=9)
    notch = np.loadtxt(detectordimension_data, skiprows=38, usecols=10)
    source_thickness = np.loadtxt(detectordimension_data, skiprows=38, usecols=11)
    end_cap_face_thickness = np.loadtxt(detectordimension_data, skiprows=38, usecols=12)
    end_cap_stand_out = np.loadtxt(detectordimension_data, skiprows=38, usecols=13)
    distance_source_detector = np.loadtxt(detectordimension_data, skiprows=38, usecols=14)
    # Ersetzen der Platzhalter 
    for i in range(2):#len(disintegrations)):
        # Platzhalter in utrwrapper-efficiency-jonny.xmac
        activity_old = "{{disintegrations}}"
        activity_new = str(int(disintegrations[i]))
        for j in range(2):#len(crystal_diameter)):
            # Platzhalter in DetectorConstruction.cc
            crystal_diameter_old = "{{cystal_diameter}}"
            crystal_diameter_new = str(crystal_diameter[j])
            crystal_length_old = "{{crystal_length}}"
            crystal_length_new = str(crystal_length[j])
            hole_diameter_old = "{{hole_diameter}}"
            hole_diameter_new = str(hole_diameter[j])
            hole_depth_old = "{{hole_depth}}"
            hole_depth_new = str(hole_depth[j])
            dead_layer_side_old = "{{dead_layer_side}}"
            dead_layer_side_new = str(dead_layer_side[j])
            dead_layer_top_old = "{{dead_layer_top}}"
            dead_layer_top_new = str(dead_layer_top[j])
            folder_old = "{{folder}}"
            folder_new = "Efficiency_" + str(k) + "_" + str(i) + str(j)
            distance_detector_end_cap_old = "{{distance_detector_end_cap}}"
            distance_detector_end_cap_new = str{{distance_detector_end_cap[j]}}
            hollow_cylinder_old = "{{hollow_cylinder}}"
            hollow_cylinder_new = str{{hollow_cylinder}}
            source_holder_old = "{{source_holder}}"
            source_holder_new = str{{source_holder}}
            notch_old = "{{notch}}"
            notch_new = str{{notch}}
            source_thickness_old = "{{source_thickness}}"
            source_thickness_new = str{{source_thickness}}
            end_cap_face_thickness_old = "{{end_cap_face_thickness}}"
            end_cap_face_thickness_new = str{{end_cap_face_thickness}}
            end_cap_stand_out_old = "{{end_cap_stand_out}}"
            end_cap_stand_out_new = str{{end_cap_stand_out}}
            activity_change(activity_old, activity_new, utrwrapper_efficiency_jonny_old_content)
            detectorconstruction_change(crystal_diameter_old, crystal_diameter_new, crystal_length_old, crystal_length_new, hole_diameter_old, hole_diameter_new, hole_depth_old, hole_depth_new, dead_layer_side_old, dead_layer_side_new, dead_layer_top_old, dead_layer_top_new, distance_detector_end_cap_old, distance_detector_end_cap_new, hollow_cylinder_old, hollow_cylinder_new, source_holder_old, source_holder_new, notch_old, notch_new, source_thickness_old, source_thickness_new, end_cap_face_thickness_old, end_cap_face_thickness_new, end_cap_stand_out_old, end_cap_stand_out_new, DetectorConstruction_old_content)
            subprocess.run(command, shell=True)
        
    
        



