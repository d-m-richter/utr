#!/usr/bin/env python3
import numpy as np
import random

################# Variation of Detector Dimension ##################

###### Known properties of the detector from data sheet GEM Series HPGe GEM20P470 ######
dead_layer_top_default = 0.7 # mmGe/Li ------------------------------ top dead layer at face of crystal, use only germanium as material
dead_layer_side_default = 0.7 # mmGe/Li ----------------------------- side dead layer of crystal, use only germanium as material
diameter_default = 50 - 2 * dead_layer_side_default# mm ------------------------------------------ diameter of crystal 
length_default = 64 - dead_layer_top_default# mm -------------------------------------------- length of crystal 
hole_diameter_default = 9.6 - 2 * 0.0003 # mm ------------------------------------ diameter of crystal hole
hole_depth_default = 51.5 - 0.0003 # mm -------------------------------------- length of crystal hole 


###### limit deviation and uncertainty from uniform distribution stored in lists for data sheet ######
limit_deviation = [0, 0.1, 0.5, 1.0] # mm  and mm^3 ----------------- (deutsch: Grenzabweichung) because the limit deviation is not known, we will try 4 different options
limit_dead_layer = [0, 0.2, 0.5, 1.0]
########################################################################################################################################################################

for i in range(len(limit_deviation)):# ----------------------------------------- data sheet properties for i in [0.0, 0.05773502691896258, 0.2886751345948129, 0.5773502691896258]
    list = [] # ---------------------------------------------- create empty list for hole depth which should have at the end 40 entries
    with open(f"detectorvariation_{i}.txt", "w") as file: # ---------- open and create .txt file to save detector dimensions with different i and j in it
        header = "crystal_diameter\tcrystal_length\thole_diameter\thole_depth\tdead_layer_top\tdead_layer_side\n"
        file.write(header) # write the above header into the first row of the .txt file
        # choose random indices
        while len(list) < 40: # ------------------------------ while loop goes only until 40 value pairs are found for each .txt file
            # choose random value from uniform distribuntion from uncertainty 
            diameter_raw = random.uniform(diameter_default - limit_deviation[i], diameter_default + limit_deviation[i])
            length_raw = random.uniform(length_default - limit_deviation[i], length_default + limit_deviation[i])
            hole_diameter = random.uniform(hole_diameter_default - limit_deviation[i], hole_diameter_default + limit_deviation[i])
            hole_depth = random.uniform(hole_depth_default - limit_deviation[i], hole_depth_default + limit_deviation[i])

            if dead_layer_side_default == dead_layer_top_default:
                dead_layer_top = random.uniform(dead_layer_top_default - limit_dead_layer[i], dead_layer_top_default + limit_dead_layer[i])
                dead_layer_side = dead_layer_top
            else:
                dead_layer_top = random.uniform(dead_layer_top_default - limit_dead_layer[i], dead_layer_top_default + limit_dead_layer[i])
                dead_layer_side = random.uniform(dead_layer_side_default - limit_dead_layer[i], dead_layer_side_default + limit_dead_layer[i])

            diameter = diameter_raw - 2 * dead_layer_side
            length = length_raw - dead_layer_top

            if dead_layer_side >= 0 and dead_layer_top >= 0: # ----------------------------- condition for dead layers to be positive
                list.append(diameter) # ------------------------------------------------- append hole_depth to list 
                "crystal_diameter\tcrystal_length\thole_diameter\thole_depth\tdead_layer_top\tdead_layer_side\n"
                line = f"{diameter} \t {length} \t {hole_diameter} \t {hole_depth} \t {dead_layer_top} \t {dead_layer_side} \n".format(1)
                file.write(line) # ------------------------------------------------------------------- write to file

    print(f"Data written to detectorvariation_{i}.txt")
