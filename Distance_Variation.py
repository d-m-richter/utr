#!/usr/bin/env python3
import numpy as np
import random
from math import pi, sqrt
#from utr_variation_GEM import hollow_cylinder_default

################# Variation of distance between crystal and source ###############

###### by mechanic produced objects ######
hollow_cylinder_default = 100 #mm
source_holder_default = 144.03 # mm --------------------------------- heigth of the source holder (deutsch: Quellenhalter)
notch_default = 1.5 # mm -------------------------------------------- (deutsch: Einkerbung) heigth of the notch in source holder

###### mechanics limit deviation and uncertainty from uniform distribution stored in lists ######
limit_deviation = [0, 0.1, 0.5, 1.0] # mm ----------------- (deutsch: Grenzabweichung) because the limit deviation is not known, we will try 4 different options 

###################################################################################################################################################################

###### thickness of radioactive source container -> source placed in the center of the container ######
source_thickness_default = 2.94 # mm --------------------------------- thickness of the source container measured with a measuring stick 

###### uncertainty for measurments with measuring stick ######
u_measuring_stick = 2 * 0.02 / sqrt(12) # mm ------------------------- (deutsch: Messschieber) uncertainty of the measuring stick with a limit deviation of 0.02 mm and through a uniform distribution
#u_measuring_stick = 0
#-------> u_measuring_stick = +\- 0.011547005383792516 mm

###################################################################################################################################################################
###### Die Endkappe und der Hohlzylinder schließen nicht bündig miteinander ab, da die Endkappe in die Mountkappe eingedreht ist. Mit einem Maßband wurde der sichtbare Teil der Endkappe ######
###### gemessen zu 132mm. Die in der Mechanik angefertigte Aluminumbodenpaltte der Bleiburg schließt bündig mit der Mountkappe ab, sodass der Hohlzylinder 132mm der Endkappe umschließt. ######
###### Eigentlich hätte die Endkappe eine Länge von 135mm. ######
end_cap_length_above_table_default = 127 # mm ------------------------------------------------------------------------- part of end cap that is not bordered by the mount cap and minus 1mm for the thickness of the window which is not 1mm in reality

###### uncertainty form measurments with Zollstock Klasse 3 Länge 2m
u_length_measurement = 2 * 1.4 / sqrt(12) # mm ------------------------------------------------------------------- (deutsch: Längenmaß) with limit deviation of 0.5 mm and uniform distribution
#u_length_measurement = 0
#-------> u_length_measurement = +\- 0.2886751345948129 mm
######################################################################################################################################################################

for j in range(len(limit_deviation)): # ------------------------------------- mechanic properties for j in [0.0, 0.05773502691896258, 0.2886751345948129, 0.5773502691896258]
    list = [] # ---------------------------------------------- create empty list for hole depth which should have at the end 40 entries
    with open(f"distancevariation_{j}.txt", "w") as file: # ---------- open and create .txt file to save detector dimensions with different i and j in it
        header = "hollow_cylinder\tsource_holder\tnotch\tsource_thickness\tend_cap_length_above_table\tdistance_source_detector\n"
        file.write(header) # write the above header into the first row of the .txt file
        # choose random indices
        while len(list) < 40: # ------------------------------ while loop goes only until 40 value pairs are found for each .txt file
        # choose random value from uniform distribuntion from uncertainty 
            hollow_cylinder = random.uniform(hollow_cylinder_default - limit_deviation[j], hollow_cylinder_default + limit_deviation[j])
            source_holder = random.uniform(source_holder_default - limit_deviation[j], source_holder_default + limit_deviation[j])
            notch = random.uniform(notch_default - limit_deviation[j], notch_default + limit_deviation[j])
            source_thickness = random.uniform(source_thickness_default - u_measuring_stick, source_thickness_default + u_measuring_stick)
            end_cap_length_above_table = random.uniform(end_cap_length_above_table_default - u_length_measurement, end_cap_length_above_table_default + u_length_measurement)
            
            distance_source_detector = hollow_cylinder + source_holder - notch + source_thickness/2 - end_cap_length_above_table
                    
            list.append(hollow_cylinder) # ------------------------------------------------- append hole_depth to list 
            "hollow_cylinder\tsource_holder\tnotch\tsource_thickness\tend_cap_length_above_table\tdistance_source_detector\n"
            line = f"{hollow_cylinder} \t {source_holder} \t {notch} \t {source_thickness} \t {end_cap_length_above_table} \t {distance_source_detector}\n".format(1)
            file.write(line) # ------------------------------------------------------------------- write to file
    print(f"Data written to distancevariation_{j}.txt")
