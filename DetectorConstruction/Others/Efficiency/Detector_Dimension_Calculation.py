import numpy as np
import random
from scipy.optimize import minimize
from math import pi, sqrt
from numpy import var, mean

################# Calculation of possible values for the crystal hole dimensions and the distance between crystal and source ###############

###### known properties of the detector from data sheet Ge-EurisysMesure_52006 ######
diameter_default = 64 # mm ------------------------------------------ diameter of crystal without dead_layer
length_default = 53 # mm -------------------------------------------- length of crystal without dead layer
crystal_active_volume = 166000 # mm^3 ------------------------------- active crystal volume, that means without dead layer
dead_layer_top_default = 1 # mmGe ----------------------------------- top dead layer at face of crystal (assumed to be 1 by default)
dead_layer_side_default = 0  # mmGe --------------------------------- side dead layer (assumed to be 0 by default, else the crystal active volume would not be right)
distance_crystal_end_cap_default = 4 # mmGe ------------------------- distance between crystal and face end cap

###### limit deviation and uncertainty from uniform distribution stored in lists for data sheet ######
limit_deviation = [0, 0.1, 0.5, 1.0] # mm ------------------------------ (deutsch: Grenzabweichung) because the limit deviation is not known, we will try 4 different options 
u_data_sheet = [] # mm ------------------------------------------------- uncertainty for detector properties from data sheet
for i in range(len(limit_deviation)): # ----------------------------- calculation of uncertainty from limit deviation through uniform distribution
    u_data_sheet.append(2 * limit_deviation[i] / sqrt(12))

################################################################################################################################################################

###### by mechanic produced objects ######
hollow_cylinder_default = 100 # mm ---------------------------------- heigth of the hollow cylinder (deutsch: Hohlzylinder)
source_holder_default = 144.03 # mm --------------------------------- heigth of the source holder (deutsch: Quellenhalter)
notch_default = 1.5 # mm -------------------------------------------- (deutsch: Einkerbung) heigth of the notch in source holder

###### mechanics limit deviation and uncertainty from uniform distribution stored in lists ######
limit_deviation_mechanics = [0, 0.1, 0.5, 1.0] # mm -------------------- (deutsch: Grenzabweichung) because the limit deviation is not known, we will try 4 different options 
u_mechanics = [] # mm -------------------------------------------------- uncertainty for by mechanic produced objects properties from data sheet
for i in range(len(limit_deviation_mechanics)): # ------------------- calculation of uncertainty from limit deviation through uniform distribution
    u_mechanics.append(2 * limit_deviation_mechanics[i] / sqrt(12))

###################################################################################################################################################################

###### thickness of radioactive source container -> source placed in the center of the container ######
source_thickness_default = 2.94 # mm ------------------------------------ half thickness of the source container measured with a measuring stick 

u_measuring_stick = 2 * 0.02 / sqrt(12) # mm ---------------------------- (deutsch: Messschieber) uncertainty of the measuring stick with a limit deviation of 0.02 mm and through a uniform distribution

###################################################################################################################################################################

###### endcap face thickness was measured 10 times with a measuring stick ######
list_end_cap_face_thickness = [0.99, 0.81, 0.63, 0.75, 0.83, 1.0, 0.90, 0.94, 0.95, 0.69] # mm ------------------- list of measured values for the thickness of the end cap face
array_end_cap_face_thickness = np.array(list_end_cap_face_thickness) # mm ---------------------------------------- convert the list into an array
end_cap_face_thickness_default = array_end_cap_face_thickness.mean() # mm ---------------------------------------- calculate the mean of the measurement series
u_std_end_cap_face_thicknes = array_end_cap_face_thickness.var() # mm -------------------------------------------- calculate the variance of the measurement series
u_end_cap_face_thickness = sqrt(u_std_end_cap_face_thicknes**2 + u_measuring_stick**2) # mm ---------------------- calculate absolute uncertainty from varaince and uncertainty from measuring stick 


###### Die Endkappe und der Hohlzylinder schließen nicht bündig miteinander ab, da die Endkappe in die Mountkappe eingedreht ist. Mit einem Maßband wurde der sichtbare Teil der Endkappe
# gemessen zu 132mm. Die in der Mechanik angefertigte Aluminumbodenpaltte der Bleiburg schließt bündig mit der Mountkappe ab, sodass der Hohlzylinder 132mm der Endkappe umschließt.
# Eigentlich hätte die ENdkappe eine Länge von 135mm. ######
end_cap_stand_out_default = 132 # mm ----------------------------------------------------------------------------- part of end cap that is not bordered by the mount cap 

u_length_measurement = 2 * 0.5 / sqrt(12) # mm ------------------------------------------------------------------- (deutsch: Längenmaß) with limit deviation of 0.5 mm and uniform distribution

#####################################################################################################################################################################

# optimization of hole dimensions --------------- muss hier auch noch eine Unsicherheit eingebaut werden?
def hole_dimensions (params, crystal_radius, crystal_length, volume_actual):
    hole_radius, hole_depth_cylinder = params # mm ------------------------------------------------------------------- parameters that we want to get out of this optimization
    volume = pi * crystal_radius**2 * crystal_length - (pi * hole_radius**2 * hole_depth_cylinder + 4/6 * pi * hole_radius**3) # mm^3 ------ volume containing the volume of a cylinder and half a sphere 
    objective = np.abs(volume - volume_actual) # mm^3 ------------------------------------------------------------------ calculate amount (Betrag?) of distance 
    return objective # -------------------------------------------------------------------------------------------- return object

######################################################################################################################################################################

distance_default_wo = hollow_cylinder_default + source_holder_default - notch_default + source_thickness_default/2 + end_cap_face_thickness_default - end_cap_stand_out_default + distance_crystal_end_cap_default # mm ------------- default distance without dead layer
distance_default_w = hollow_cylinder_default + source_holder_default - notch_default + source_thickness_default/2 + end_cap_face_thickness_default - end_cap_stand_out_default + distance_crystal_end_cap_default + dead_layer_top_default # mm ------------- default distance with dead layer

print(distance_default_wo, distance_default_w)

# calculation of distance and its uncertainty ----------------------- default value for distance should be 117mm without dead layer
def distance_source_crystal (hollow_cylinder_actual, source_holder_actual, notch_actual, source_thickness_actual, end_cap_stand_out_actual, end_cap_face_thickness_actual, distance_crystal_end_cap_actual, dead_layer_top_actual, u_mechanics, u_measuring_stick, u_length_measurement, u_end_cap_face_thickness, u_data_sheet):
    distance_source_detector = hollow_cylinder_actual + source_holder_actual - notch_actual + source_thickness_actual/2 - end_cap_stand_out_actual + end_cap_face_thickness_actual + distance_crystal_end_cap_actual + dead_layer_top_actual  # mm ----- calculate distance
    u_distance_source_detector = sqrt(3 * u_mechanics**2 + (u_measuring_stick/2)**2 + u_length_measurement**2 + u_end_cap_face_thickness**2 + u_data_sheet**2) # mm ----- distance uncertainty (absolute)
    distance_actual = random.uniform(distance_source_detector - u_distance_source_detector, distance_source_detector + u_distance_source_detector) # mm ------ calculate random distance from uniform distribution
    return distance_actual # --------------------------------------------------------------------------------------- return random distance from uniform distribution

########################################################################################################################################################################

for i in range(len(u_data_sheet)):# ----------------------------------------- data sheet properties for i in [0.0, 0.05773502691896258, 0.2886751345948129, 0.5773502691896258]
    for j in range(len(u_mechanics)): # ------------------------------------- mechanic properties for j in [0.0, 0.05773502691896258, 0.2886751345948129, 0.5773502691896258]
        hole_depth_list = [] # ---------------------------------------------- create empty list for hole depth which should have at the end 40 entries
        with open(f"detectordimension_{i}{j}.txt", "w") as file: # ----------- open and create .txt file to save detector dimensions with different i and j in it
            header = "crystal_diameter\tcrystal_length\tactive_volume\thole_diameter\thole_depth\tdead_layer_side\tdead_layer_top\tdistance_detector_end_cap\thollow_cylinder\tsource_holder\tnotch\tsource_thickness\tend_cap_face_thickness\tend_cap_stand_out\tdistance_source_detector\n"
            file.write(header) # write the above header into the first row of the .txt file
            # choose random indices
            while len(hole_depth_list) < 40: # ------------------------------ while loop goes only until 40 value pairs are found for each .txt file
                # choose random value from uniform distribuntion from uncertainty 
                diameter_actual = random.uniform(diameter_default - u_data_sheet[i], diameter_default + u_data_sheet[i])
                length_actual = random.uniform(length_default - u_data_sheet[i], length_default + u_data_sheet[i])
                volume_actual = random.uniform(crystal_active_volume - u_data_sheet[i]**3, crystal_active_volume + u_data_sheet[i]**3) # Nachfragen: ist das hier richtig?
                dead_layer_top_actual = random.uniform(dead_layer_top_default - u_data_sheet[i], dead_layer_top_default + u_data_sheet[i])
                dead_layer_side_actual = random.uniform(dead_layer_side_default - u_data_sheet[i], dead_layer_side_default + u_data_sheet[i])
                distance_crystal_end_cap_actual = random.uniform(distance_crystal_end_cap_default - u_data_sheet[i], distance_crystal_end_cap_default + u_data_sheet[i])
                hollow_cylinder_actual = random.uniform(hollow_cylinder_default - u_mechanics[j], hollow_cylinder_default + u_mechanics[j])
                source_holder_actual = random.uniform(source_holder_default - u_mechanics[j], source_holder_default + u_mechanics[j])
                notch_actual = random.uniform(notch_default - u_mechanics[j], notch_default + u_mechanics[j])
                source_thickness_actual = random.uniform(source_thickness_default - u_measuring_stick, source_thickness_default + u_measuring_stick)
                end_cap_face_thickness_actual = random.uniform(end_cap_face_thickness_default - u_end_cap_face_thickness, end_cap_face_thickness_default + u_end_cap_face_thickness)
                end_cap_stand_out_actual = random.uniform(end_cap_stand_out_default - u_length_measurement, end_cap_stand_out_default + u_length_measurement)


                if dead_layer_side_actual >= 0 and dead_layer_top_actual >= 0: # -------------------------- condition for dead layers to be positive
                    # calculation of crystal diameter and length under consideration of dead layers
                    crystal_diameter = diameter_actual - 2 * dead_layer_side_actual
                    crystal_radius = 0.5 * crystal_diameter
                    crystal_length = length_actual - dead_layer_top_actual

                    # optimization of hole diameter and depth
                    initial_guess = [random.uniform(3, 10), random.uniform(10, 45)] # ----------------------- initial guess for hole radius and hole cylinder depth
                    result = minimize(hole_dimensions, initial_guess, args=(crystal_radius, crystal_length, volume_actual), method = 'Nelder-Mead') # ----- minimizing the difference between calculated volume and active volume
                    hole_radius, hole_depth_cylinder = result.x # -------------------------------------------- get results for hole parameters
                    hole_diameter = 2 * hole_radius # -------------------------------------------------------- calculate diameter from radius   
                    hole_depth = hole_depth_cylinder + hole_radius # ----------------------------------------- calculate the complete depth of the hole 

                    # calculation of distance between source and crystal
                    distance_source_detector_actual = distance_source_crystal(hollow_cylinder_actual, source_holder_actual, notch_actual, source_thickness_actual, end_cap_stand_out_actual, end_cap_face_thickness_actual, distance_crystal_end_cap_actual, dead_layer_top_actual, u_mechanics[j], u_measuring_stick, u_length_measurement, u_end_cap_face_thickness, u_data_sheet[i])
                    
                    # conditions for writing values to .txt file
                    if hole_depth <= (53 - 5) and hole_depth > hole_diameter and hole_diameter >= 6: # ------- condictions for hole_depth and hole_diameter (derived from examples of other coaxial HPGe crystals)
                        hole_depth_list.append(hole_depth) # --------------------- append hole_depth to list 
                        "crystal_diameter\tcrystal_length\tactive_volume\thole_diameter\thole_depth\tdead_layer_side\tdead_layer_top\tdistance_detector_end_cap\thollow_cylinder\tsource_holder\tnotch\tsource_thickness\tend_cap_face_thickness\tend_cap_stand_out\tdistance_source_detector\n"
                        line = f"{crystal_diameter} \t {crystal_length} \t {volume_actual} \t {hole_diameter} \t {hole_depth} \t {dead_layer_side_actual} \t {dead_layer_top_actual} \t {distance_crystal_end_cap_actual} \t {hollow_cylinder_actual} \t {source_holder_actual} \t {notch_actual} \t {source_thickness_actual} \t {end_cap_face_thickness_actual} \t {end_cap_stand_out_actual} \t {distance_source_detector_actual}\n".format(1)
                        file.write(line) # ---------------------- write to file

        print(f"Data written to detectordimension_{i}{j}.txt")




