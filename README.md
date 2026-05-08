# Geant4 simulation of a gamma spectrometry setup for radiation safety purposes

This is a [Geant4](https://geant4.web.cern.ch/) [[1]](#ref-g4_1) [[2]](#ref-g4_2) [[3]](#ref-g4_3) simulation of gamma spectrometry with high purity germanium detectors, which are used in the radiation safety department at IKP TU Darmstadt, Germany. It is used to calculate the energy and geometry depending efficiencies of activated radioactive objects from experimental use like beam times with S-DALINAC. This simulation code is based on the UTR Geant4 simulation of U. Friman-Gayer, J. Kleemann and O. Papst ([![DOI](https://zenodo.org/badge/79936359.svg)](https://zenodo.org/badge/latestdoi/79936359)) and was adapted for use in radiation protection. For more information on the UTR simulation, have a look in "README_utr_original.md". This README describes only the changes to the UTR simulation.

# Table of Contents

 1. [Requirements](#requirements)
 2. [Detector Construction](#detectorconstruction)
 3. [Usage and Visualization](#usage)
 4. [Physics](#physics)
 5. [GDML](#gdml)
 6. [Templates](#templates) 
 7. [Compilations](#compilations)
 8. [Output](#output)
 9. [Starting a run](#startup)
 10. [References](#references)

## 1 Requirements <a name="requirements"></a>

For this simulation the Geant4 Version 10.7.4 was used. Other versions can be used, but for the simulation to run smoothly some changes in the code have to be made.
The output files are generated as root and also as text files. So python can be used and is also required for the utrwrapper.

## 2 Detector Construction <a name="detectorconstruction"></a>

In the file `DetectorConstruction/Others/Efficiency/DetectorConstruction.cc` the whole setup of the simulation is set. It consists of three parts. First the detector is constructed by using the `scr/HPGe_Coaxial` class for a coaxial p-typ HPGe detector with a inner and outer dead layer and appointing values to the detector properties.

In the second part the geometries with the to be measured activities are declared. The construction of the geometries can either be done directly in the `DetectorConstruction`.cc file or in extra files like for example it was done with a hollow cylinder in Hohlzylinder.hh and then include this file in the `DetectorConstruction` with the command `#include "Hohlzylinder.hh"` and placing the hollow cylinder in the constructed world with
``
Hohlzylinder *hohlzylinder = new Hohlzylinder(world_logical);
hohlzylinder->Put(x, y, z, rot_x, rot_y, rot_z);
``
The same applies to a lead shielding if to be inlcuded.

In the last part the sensitive detector volume is declared to be the germanium crystal (without dead layer) in the HPGe detector with `SetSensitiveDetector()`. Here it can also be declared, what should be measured (here it is the deposited energy in the crystal `EnergyDepositionSD`). 

### 2.1 Set Material <a name="setmaterial"></a>



## 4 Radioactive Source Construction <a name="physics"><a>

The radioactivity of the to be measured geometry volume is declared in `macros/examples/utrwrapper-efficiency-jonny.xmac`. There are two different ways to define the radiation. If only gammas have to be simulated then the following part can be taken:  
``
/gps/particle gamma
/gps/ene/type Mono

/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/radius r mm
/gps/pos/halfz h/2 mm

/gps/ang/type iso
/gps/pos/confine Holzylinder

/gps/pos/centre x y z mm
``
Here a gamma source is constructed in form of a cylinder with radius r and height h. With the confine `/gps/pos/command` this cylindric volume can be confined to a defined logical volume (name of logical volume = `Hohlzylinder`) so that only from this logical volume gammas are emitted. The command `/control/alias beamOnStatistics N` can be used to set the number of gammas N to be simulated. With the command `/control/foreach macros/examples/genericLoopable.mac loopVar E # MeV` the energy E of the gammas can be set. It is also possible to set more than one energy (separate values only with spaces, no commas).   

If, instead, the complete radioactive decay is to be simulated, the particle has to be set to ion (`/gps/particle gamma`) and the command `/gps/ene/type Mono` has to be commented out (`#/gps/ene/type Mono`). Also the command `/control/foreach macros/examples/genericLoopable.mac loopVar E # MeV` must be replaced by
``
/control/alias loopVar "Z A"
/control/execute macros/examples/genericLoopable.mac
``
With `/control/alias loopVar "Z A"` the proton number Z and the mass number A of the ion are set. 

If a decay chain like it is the case for Am-241 is to be simulated, but one only wants to see the gamma lines from Am-241 then the command `/process/had/rdm/nucleusLimits 240 250 94 96` has to be added:
``
/control/alias loopVar "95 241"
/process/had/rdm/nucleusLimits 240 250 94 96
/control/execute macros/examples/genericLoopable.mac
``
In this specific example, only the decay of Am-241 is permitted. Subsequent decays, such as that of Np-237, are suppressed. This method ensures that only primary decay processes are evaluated.

The physics library `G4RadioactiveDecayPhysics` was added to the already included libraries in utr.

## 5 GDML <a name="gdml"></a>

For the implementation of more complex geometries it is useful to download [GDML](https://github.com/JeffersonLab/gdml) [[4]](#ref-gdml) into this folder. With this gdml files can be read by the `G4GDMLParser` and placed in the world of the simulation with
```
#include "G4GDMLParser.hh "
G4GDMLParser parser;
G4String gdmlFileName;
parser.Read(gdmlFileName); // need path to gdml file
G4VPhysicalVolume ∗ gdmlWorld = parser.GetWorldVolume();
G4LogicalVolume ∗ Fuelvolumen_Logical = gdmlWorld−>GetLogicalVolume();
```
To generate gdml files of complex geometries that are for example in a `STEP` format a suitable converter is needed like the Python script [GUIMesh3](https://github.com/MPintoSpace/GUIMesh3) [[5]](#ref-guimesh).

The gdml files that are used for the simulation are written into the folder `DetectorConstruction/Others/Efficiency/Volumes`. 

## 10 References <a name="references"></a>

<a name="ref-g4_1">[1]</a> S. Agostinelli *et al.*, “GEANT4 - a simulation toolkit”, Nucl. Inst. Meth. A **506.3**, 250 (2003). [`doi:10.1016/S0168-9002(03)01368-8`](https://doi.org/10.1016/S0168-9002(03)01368-8).
<a name="ref-g4_2">[2]</a> J. Allison *et al.*, “GEANT4 developments and applications”, IEEE Transactions on Nuclear Science, **53.1**, 270 (2006). [`doi:10.1109/TNS.2006.869826`](https://doi.org/10.1109/TNS.2006.869826).
<a name="ref-g4_3">[3]</a> J. Allison *et al.*, “Recent developments in GEANT4”, Nucl. Inst. Meth. A **835**, 186 (2016). [`doi:10.1016/j.nima.2016.06.125`](https://doi.org/10.1016/j.nima.2016.06.125).
<a name="ref-gdml">[4]</a> Jefferson Lab, gdml - Geometry Description Markup Language for Geant4, Version 3.1.6, 2017. (https://github.com/JeffersonLab/gdml).
<a name="ref-guimesh">[5]</a> M. G. A. Pinto, GUIMesh, 21.06.2024. (https://github.com/MPintoSpace/GUIMesh3).
