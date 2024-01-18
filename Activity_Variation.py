#!/usr/bin/env python3
import numpy as np
import datetime
import random
from math import sqrt, pi, exp, log
#from utr_variation_GEM import Live_time

######################################## Calculation of source activity #################################

# half life time
T_12 = 30.08 #a for Cs-137 (from NuDat)

# emission propability
P_662 = 0.851 # from Nuklidsuche

# activity
#A_t0 = 379000 #Bq Cs-137 calibration source (P-1177) from 01.06.2016
A_t0 = 18540 #Bq Cs-137 calibration (P-1030) from 01.01.2016

# uncertainty of activity A_t0 with lower limit - 10% * A and upper limit 30% * A in rectangular distribution
#expectation_value = (A_t0 * (1 - 0.1) + A_t0 * (1 + 0.3)) / 2
#u_A_t0 = abs(A_t0 * (1 - 0.1) - A_t0 * (1 + 0.3)) / sqrt(12)
u_A_t0 = A_t0 * 0.1 # Bq --------------- for 10% activity uncertainty


# time calculation for activity
dt1 = datetime.datetime(2016,1,1) 
dt2 = datetime.datetime(2023,11,4) # alte 100mm Messung 13.09.2023
time_diff = dt2 - dt1 
tdays = time_diff.days

# activity
A_tdays = A_t0 * exp(-log(2)/(T_12 * 365) * tdays)

#print(A_tdays)

# uncertainty of activity A_tdays
u_A_tdays = exp(-log(2)/(T_12 * 365) * tdays) * u_A_t0

# Live_time is already corrected time
Live_time = 80 #s

disintegration_from_activity = []

with open("activity.txt", "w") as file:
    header = "activity\tdisintegration\n"
    file.write(header)
    while len(disintegration_from_activity) < 40:
        #i = random.randint(0,len(activity))
        activity = random.uniform(A_tdays - 3 * u_A_tdays, A_tdays + 3 * u_A_tdays)
        #disintegration = activity[i] * Live_time * P_662
        disintegration = activity * Live_time * P_662
        disintegration_from_activity.append(disintegration)
        # write values to file: activity in first column and disintegration in second column
        #line = f"{activity[i]:2f} \t {int(disintegration):d}\n".format(1)
        line = f"{activity:2f} \t {int(disintegration):d}\n".format(1)
        file.write(line)


print("Data written to activity.txt")
