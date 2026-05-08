/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"

#include "HPGe_Coaxial.hh"               // <- ?
#include "HPGe_Coaxial_Properties.hh"    // <- ?
#include "Units.hh"                      // <- ?

// Adapter
#include "Adapter.hh"

// ProbenhalterLang
#include "ProbenhalterLang.hh"

// ProbenhalterKurz
#include "ProbenhalterKurz.hh"

// Marinelli-Becher
#include "Marinelli_Becher.hh"

// Rundflasche
#include "PlasticBottle.hh"

// Quellenhalter
#include "Quellenhalter.hh"

// Hohlzylinder
#include "Hohlzylinder.hh"

// Filterhalter
#include "Filterhalter.hh"

// Petrischale Ensemble
#include "Folienmethode.hh"

// Bleiburg
#include "Lead_Castle_for_GEM20P470.hh"
#include "Bleiburg.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


// #include "G4UnitsTable.hh"
#include "utrConfig.h"
#include <iostream>
#include <array>

#include "G4GDMLParser.hh"
#include "G4TessellatedSolid.hh"


/*
 * Simple geometry to determine the efficiency of a detector implemented in this DetectorConstruction.
 * 
 * The simulation uses the ParticleSD detector type, which records particle information at any time when a particle propagates into a new target segment for the first time (or when it is created there).
 *
 * The detector is a p-type coaxial HPGe detector with a relative efficiency of 24 % at 1332 keV from ORTEC.
 */

G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Setup Properties *****************/
  // There is no filter

  // In this case distance_source_detector describes the distance between end cap and zero point
  const double distance_source_detector = 0.; // 100. * mm;    // <------ adjust distance from source to detector
//  const G4String filter_material_name = "G4_Pb";     // <------ adjust filter material
//  const double filter_thickness = 0.5 * mm;           // <------ adjust filter thickness

  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material *Li = nist->FindOrBuildMaterial("G4_Li");
  G4Material *B = nist->FindOrBuildMaterial("G4_B");
 // G4Material *PLA = nist->FindOrBuildMaterial("G4_PLA");
  G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material *Ge = nist->FindOrBuildMaterial("G4_Ge");
  G4Material *Mylar = nist->FindOrBuildMaterial("G4_MYLAR");

  //Epoxy from https://www.phenix.bnl.gov/~suhanov/ncc/geant/rad-source/src/ExN03DetectorConstruction.cc
  //G4double density = 1.2*g/cm3;
  //G4Material* Epoxy = new G4Material("Epoxy" , density, 4);
  //Epoxy->AddElement(nist->FindOrBuildElement("H"), 32);
  //Epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
  //Epoxy->AddElement(nist->FindOrBuildElement("N"), 2);
  //Epoxy->AddElement(nist->FindOrBuildElement("O"), 4);
  
  
  /***************** Detector Properties *****************/

  HPGe_Coaxial_Properties HPGe_GEM20P470_props;
  HPGe_GEM20P470_props.detector_radius = 0.5 * {{crystal_diameter}} * mm; // 0.5 * 50 mm
  HPGe_GEM20P470_props.detector_length = {{crystal_length}} * mm; // 64 mm 
  HPGe_GEM20P470_props.detector_face_radius = 8. * mm; // {{face_radius}} * mm; // 8 mm nominal radius
  HPGe_GEM20P470_props.detector_back_radius = 0. * mm; // {{back_radius}} * mm; // not specified 
  HPGe_GEM20P470_props.hole_radius = 0.5 * {{hole_diameter}} * mm; // 9.6 * mm; 
  HPGe_GEM20P470_props.hole_depth = {{hole_depth}} * mm; // 51.5 * mm;
  HPGe_GEM20P470_props.hole_face_radius = 5. * mm; // {{hole_face_radius}} // 5. * mm
  HPGe_GEM20P470_props.dead_layer_side = {{dead_layer_side}} * mm; // 0.7 * mm Ge/Li --> is ok to use Ge density
  HPGe_GEM20P470_props.dead_layer_top = {{dead_layer_top}} * mm; // 0.7 * mm Ge/Li --> is ok to use Ge density
  HPGe_GEM20P470_props.dead_layer_material = "G4_Ge"; 
  HPGe_GEM20P470_props.hole_dead_layer = 0.0003 * mm; // {{hole_dead_layer}} * mm; // 0.3 * micron Ge/B --> is ok to use Ge density
  HPGe_GEM20P470_props.hole_dead_layer_material = "G4_Ge"; 
  HPGe_GEM20P470_props.mount_cup_thickness = 0.8 * mm; // 0.8 * mm Al
  HPGe_GEM20P470_props.mount_cup_base_thickness = 3. * mm; // 3 * mm Al
  HPGe_GEM20P470_props.mount_cup_material = "G4_Al";
  HPGe_GEM20P470_props.mount_cup_face_material = "G4_Al";
  HPGe_GEM20P470_props.mount_cup_upper_window_thickness = 0.03 * mm; // {{mount_cup_upper_face_thickness}}; // 0.03 * mm Al
  HPGe_GEM20P470_props.mount_cup_upper_window_material = "G4_Al"; // 0.03 * mm Al
  HPGe_GEM20P470_props.mount_cup_under_window_thickness = 0.03 * mm; // {{mount_cup_under_face_thickness}}; // 0.03 * mm Mylar
  HPGe_GEM20P470_props.mount_cup_under_window_material = "G4_MYLAR"; // 0.03 * mm Mylar
  HPGe_GEM20P470_props.mount_cup_face_thickness = HPGe_GEM20P470_props.mount_cup_upper_window_thickness + HPGe_GEM20P470_props.mount_cup_under_window_thickness;
  HPGe_GEM20P470_props.end_cap_outer_radius = 0.5 * 70. * mm; // {{end_cap_radius}} * mm; // 70 * mm Al
  HPGe_GEM20P470_props.end_cap_to_crystal_gap_front = {{end_cap_to_crystal_gap_front}} * mm; // {{distance_detector_end_cap}} * mm; // 3 * mm
  HPGe_GEM20P470_props.end_cap_thickness = 1. * mm; // {{end_cap_thickness}} * mm; // 1 * mm Al
  HPGe_GEM20P470_props.end_cap_window_thickness = {{end_cap_window_thickness}} * mm; // {{end_cap_face_thickness}} * mm; // 1 * mm Al
  HPGe_GEM20P470_props.end_cap_length = 134. * mm; // {{end_cap_stand_out}}; // 134 * mm  Al
  HPGe_GEM20P470_props.end_cap_material = "G4_Al"; // 
  HPGe_GEM20P470_props.end_cap_window_material = "G4_Al"; // 
  HPGe_GEM20P470_props.cold_finger_radius = 0.5 * 4. * mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
  HPGe_GEM20P470_props.cold_finger_material = "G4_Cu"; // Estimated
  HPGe_GEM20P470_props.connection_length = 1.5 * inch;
  HPGe_GEM20P470_props.connection_radius = 0.75 * inch; // Estimated
  HPGe_GEM20P470_props.dewar_offset = 0. * inch;
  HPGe_GEM20P470_props.connection_material = "G4_Al"; // Estimated
  HPGe_GEM20P470_props.dewar_length = 12.5 * inch;
  HPGe_GEM20P470_props.dewar_outer_radius = 4.5 * inch;
  HPGe_GEM20P470_props.dewar_wall_thickness = 5. * mm; // Estimated
  HPGe_GEM20P470_props.dewar_material = "G4_Al"; // Estimated
  HPGe_GEM20P470_props.hole_face_radius = HPGe_GEM20P470_props.hole_radius; // Estimated to be the same as hole radius
  HPGe_GEM20P470_props.cold_finger_penetration_depth = HPGe_GEM20P470_props.hole_depth - 5. * mm; // Estimated
  HPGe_GEM20P470_props.end_cap_to_crystal_gap_side =
      HPGe_GEM20P470_props.end_cap_outer_radius -
      HPGe_GEM20P470_props.end_cap_thickness -
      HPGe_GEM20P470_props.mount_cup_thickness -
      HPGe_GEM20P470_props.detector_radius -
      HPGe_GEM20P470_props.dead_layer_side; // Calculated from outer radius and other given dimensions
  HPGe_GEM20P470_props.mount_cup_length = 94. * mm; // {{mount_cup_length}} * mm; // 94 * mm Al
 //     HPGe_GEM20P470_props.end_cap_length -
 //     HPGe_GEM20P470_props.end_cap_thickness -
 //     HPGe_GEM20P470_props.end_cap_to_crystal_gap_front; // Calculated from end cap length


  /***************** World Volume *****************/

  const double world_x =  1. *m; //HPGe_GEM20P470_props.dewar_outer_radius;
  const double world_y =  1. *m; //HPGe_GEM20P470_props.dewar_outer_radius;
  const double world_z =  1. *m; //2 * HPGe_GEM20P470_props.dewar_length + distance_source_detector;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, air, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);


  /******************** Detector ******************/

  HPGe_Coaxial *HPGe_GEM20P470= new HPGe_Coaxial(world_logical, "Detector_logical");
  (*HPGe_GEM20P470).setProperties(HPGe_GEM20P470_props);
  //HPGe_GEM20P470.useDewar();
  //(*HPGe_GEM20P470).Add_Filter(filter_material_name, filter_thickness, HPGe_GEM20P470_props.detector_radius + 2 *HPGe_GEM20P470_props.dewar_wall_thickness);
  (*HPGe_GEM20P470).Construct(G4ThreeVector(0., 0., 0), 0, twopi, distance_source_detector, 0);

  /***************** Test ************************************************/

  //G4double testzylinder_position = -150.0 * mm;

  //G4double density = {{density}} * g/cm3;
  //G4Material *AlO = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  //G4Material* AlO_new = nist->BuildMaterialWithNewDensity("G4_ALUMINIUM_OXIDE_new","G4_ALUMINUM_OXIDE",density);

  //G4Tubs *testzylinder_solid = new G4Tubs("Testzylinder_Solid", 0 *mm, 0.5 * 60.0 *mm, 0.5 * 150 *mm, 0. *deg, 360. *deg);
  //G4LogicalVolume *testzylinder_logical = new G4LogicalVolume(testzylinder_solid, AlO_new, "Tetszylinder_Logical");
  //G4PVPlacement *testzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, testzylinder_position), testzylinder_logical, "Testzylinder", world_logical, false, 0);

  /***************** Petrischale Ensemble ************************************************/

  //Petrischale *petrischale = new Petrischale(world_logical);
  //petrischale->Put(0., 0., 112.46 - (70.0 + 42.0) - 6.5, 180 * deg, 0., 0.);
///*
  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateX(180);
  rot->rotateY(180);
  rot->rotateZ(180);

//  G4Tubs *tub1_solid = new G4Tubs("Tub1_Solid", 0 *mm, 0.5 * 90. *mm, 0.5 * 14.75 *mm, 0. *deg, 360. *deg); 
//  G4Tubs *tub2_solid = new G4Tubs("Tub2_Solid", 0 *mm, 0.5 * 80. *mm, 0.5 * 14.75 *mm, 0. *deg, 360. *deg); 
//  G4SubtractionSolid *petrischale_solid = new G4SubtractionSolid("Petrischale_Solid", tub1_solid, tub2_solid, 0, G4ThreeVector(0,0,-0.6));
//  G4LogicalVolume *petrischale_logical = new G4LogicalVolume(petrischale_solid, polyethylene, "Petrischale_Logical");
//  G4PVPlacement *petrischale_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 112.46 - (70.0 + 42.0) - 6.5 - 14.75/2.), petrischale_logical, "Petrischale", world_logical, false, 0);

//  G4Tubs *tub3_solid = new G4Tubs("Tub3_Solid", 0 *mm, 0.5 * 100. *mm, 0.5 * 8.6 *mm, 0. *deg, 360. *deg); // 14.15
//  G4Tubs *tub4_solid = new G4Tubs("Tub4_Solid", 0 *mm, 0.5 * 90. *mm, 0.5 * 8.6 *mm, 0. *deg, 360. *deg); // 14.15
//  G4SubtractionSolid *petrischale_deckel_solid = new G4SubtractionSolid("Petrischale_Deckel_Solid", tub3_solid, tub4_solid, 0, G4ThreeVector(0,0,0.6));
  
  //G4UnionSolid *petrischale_solid = new G4UnionSolid("Petrischale_Solid", petrischale_bottom_solid, petrischale_top_solid, 0, G4ThreeVector(0,0,-3.675));

//  G4LogicalVolume *petrischale_deckel_logical = new G4LogicalVolume(petrischale_deckel_solid, polyethylene, "Petrischale_Deckel_Logical");
//  G4PVPlacement *petrischale_deckel_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 112.46 - (70.0 + 42.0) - 6.5 - 14.75 + 3.7), petrischale_deckel_logical, "Petrischale_Deckel", world_logical, false, 0);

//  Fuelvolumen *fuelvolumen = new Fuelvolumen(world_logical);
//  fuelvolumen->Put(0., 0., 112.46 - (70.0 + 42.0) - 6.5 - 0.6, 180 * deg, 0., 0.); //6.5

//  Folienhalterung *folienhalterung = new Folienhalterung(world_logical);
//  folienhalterung->Put(0., 0., 112.46 - (70.0 + 42.0 + 100.0), 180 * deg, 0., 0.);

//  Zylinderhalterung *zylinderhalterung = new Zylinderhalterung(world_logical);/
//  zylinderhalterung->Put(0., 0., 112.46 - (70.0 + 42.0 + 100.0) - 8.5, 180 * deg, 0., 0.);
//*/
/*
  G4double petrischale_position = -(0.5 * 246.5) * mm;

  G4double density = (1689.01 / 2198) * g/cm3;
  G4Material *plastik = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Material *water = nist->FindOrBuildMaterial("G4_WATER");
  G4Material *AlO = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  G4Material* AlO_new = nist->BuildMaterialWithNewDensity("G4_ALUMINIUM_OXIDE_new","G4_ALUMINUM_OXIDE",density);

  G4Tubs *petrischale_outer_solid = new G4Tubs("Petrischale_outer_Solid", 0 *mm, 0.5 * 120 *mm, 0.5 * 246.5 *mm, 0. *deg, 360. *deg);
  
  G4Tubs *petrischale_inner_solid = new G4Tubs("Petrischale_inner_Solid", 0 *mm, 0.5 * 117.5 *mm, 0.5 * 244 *mm, 0. *deg, 360. *deg); // 14.15
  G4LogicalVolume *petrischale_inner_logical = new G4LogicalVolume(petrischale_inner_solid, AlO_new, "Petrischale_inner_Logical");
  G4PVPlacement *petrischale_inner_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position), petrischale_inner_logical, "Petrischale_inner", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys1 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 15.35 *mm), petrischale_inner_logical, "Petrischale_inner1", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys2 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 2 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner2", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys3 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 3 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner3", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys4 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 4 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner4", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys5 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 5 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner5", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys6 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 6 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner6", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys7 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 7 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner7", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys8 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 8 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner8", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys9 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 9 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner9", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys10 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 10 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner10", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys11 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 11 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner11", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys12 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 12 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner12", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys13 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 13 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner13", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys14 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 14 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner14", world_logical, false, 0);
  //G4PVPlacement *petrischale_inner_phys15 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 15 * 15.35 *mm), petrischale_inner_logical, "Petrischale_inner15", world_logical, false, 0);

  G4SubtractionSolid * petrischale_solid = new G4SubtractionSolid("Petrischale_Solid", petrischale_outer_solid, petrischale_inner_solid);
  G4LogicalVolume *petrischale_logical = new G4LogicalVolume(petrischale_solid, plastik, "Petrischale_Logical");
  G4PVPlacement *petrischale_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position), petrischale_logical, "Petrischale", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys1 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 15.35 *mm), petrischale_logical, "Petrischale1", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys2 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 2 * 15.35 *mm), petrischale_logical, "Petrischale2", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys3 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 3 * 15.35 *mm), petrischale_logical, "Petrischale3", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys4 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 4 * 15.35 *mm), petrischale_logical, "Petrischale4", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys5 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 5 * 15.35 *mm), petrischale_logical, "Petrischale5", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys6 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 6 * 15.35 *mm), petrischale_logical, "Petrischale6", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys7 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 7 * 15.35 *mm), petrischale_logical, "Petrischale7", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys8 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 8 * 15.35 *mm), petrischale_logical, "Petrischale8", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys9 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 9 * 15.35 *mm), petrischale_logical, "Petrischale9", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys10 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 10 * 15.35 *mm), petrischale_logical, "Petrischale10", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys11 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 11 * 15.35 *mm), petrischale_logical, "Petrischale11", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys12 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 12 * 15.35 *mm), petrischale_logical, "Petrischale12", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys13 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 13 * 15.35 *mm), petrischale_logical, "Petrischale13", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys14 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 14 * 15.35 *mm), petrischale_logical, "Petrischale14", world_logical, false, 0);
  //G4PVPlacement *petrischale_phys15 = new G4PVPlacement(0, G4ThreeVector(0, 0, petrischale_position - 15 * 15.35 *mm), petrischale_logical, "Petrischale15", world_logical, false, 0);

*/
  /***************** Folienstandard ************************************************/
///*
  G4double folie_position = 112.46 - (70.0 + 42.0 + 70.0) - 6.5 -(0.5 * 0.8) * mm; // - 15.35 6.5

  //G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

  G4Tubs *folie_solid = new G4Tubs("Folie_Solid", 0 *mm, 0.5 * 100.0 *mm, 0.5 * 0.8 *mm, 0. *deg, 360. *deg);
  G4LogicalVolume *folie_logical = new G4LogicalVolume(folie_solid, polyethylene, "Folie_Logical");
  G4PVPlacement *folie_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, folie_position), folie_logical, "Folie", world_logical, false, 0);
//*/
  /***************** Marinelli-Becher *********************************/

  //MarinelliBeaker *marinelliBeaker = new MarinelliBeaker(world_logical);
  //marinelliBeaker->Put(0., 0., 110.);

  /***************** Marinelli-Becher Deckel *********************************/

  //MarinelliDeckel *marinelliDeckel = new MarinelliDeckel(world_logical);
  //marinelliDeckel->Put(0., 0., 18.);

  /***************** Rundflasche *********************************/

  //G4double plastic_bottle_position = -(0) * mm;

  //Plastic_Bottle *plasticbottle = new Plastic_Bottle(world_logical);
  //plasticbottle->Put(0., 0., plastic_bottle_position, 180 * deg, 0., 0.); // -3.5

  //Plastic_Bottle_Filled *plasticbottlefilled = new Plastic_Bottle_Filled(world_logical);
  //plasticbottlefilled->Put(0., 0., plastic_bottle_position - 1.25, 180 * deg, 0., 0.); // -4.75

  //Bottle_Holder *bottleholder = new Bottle_Holder(world_logical);
  //bottleholder->Put(0., 0., 41.0, 180 * deg, 0., 0.); // 48.5

  /***************** Quellenhalter *********************************/

  //G4double hohlzylinder_position = {{hollow_cylinder_position}};
  //G4double quellenhalter_position = hohlzylinder_position - {{hollow_cylinder}};

  //Quellenhalter *quellenhalter = new Quellenhalter(world_logical);
  //quellenhalter->Put(0., 0., quellenhalter_position, 180. *deg, 0., 0.);

  /***************** Hohlzylinder *********************************/

  //Hohlzylinder *hohlzylinder = new Hohlzylinder(world_logical);
  //hohlzylinder->Put(0., 0., 128.5, 180. *deg, 0., 0.);

  // In dem Hohlzylinder sind die Höhen von der Marinellihalterung, dem 30mm und 40mm hohen Hohlzylinder enthalten.
/*
  G4Tubs *hohlzylinder_solid = new G4Tubs("Hohlzylinder_Solid", 0.5 * 81.0 *mm, 0.5 * 102.0 *mm, 0.5 * (70.0 + 42.0 + 100.0) *mm, 0. *deg, 360. *deg); // für die Simulation mit mehreren Petrischalen wurde 117.5 mm für den Durchmesser verwendet
  G4LogicalVolume *hohlzylinder_logical = new G4LogicalVolume(hohlzylinder_solid, polyethylene, "Hohlzylinder_Logical");
  G4PVPlacement *hohlzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 112.46 - (70.0 + 42.0 + 100.0)/2.), hohlzylinder_logical, "Hohlzylinder", world_logical, false, 0);
*/
  /***************** Hohlzylinder für GEM *********************************/

  //Hohlzylinder_GEM *hohlzylinder_GEM = new Hohlzylinder_GEM(world_logical);
  //hohlzylinder_GEM->Put(0., 0., 128.5, 180. *deg, 0., 0.);

  /***************** Plastikzylinder für Kalibrierquelle P-1030 *********************************/

  //G4double plastikzylinder_position = 0. * mm;

  //G4Tubs *plastikzylinder_solid = new G4Tubs("Plastikzylinder_Solid", 0 *mm, 0.5 * 25.03 *mm, 0.5 * {{source_thickness}} *mm, 0. *deg, 360. *deg);
  //G4LogicalVolume *plastikzylinder_logical = new G4LogicalVolume(plastikzylinder_solid, Epoxy, "Plastikzylinder_Logical");
  //G4PVPlacement *plastikzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, plastikzylinder_position), plastikzylinder_logical, "Plastikzylinder", world_logical, false, 0);

  /***************** Bleiburg *********************************/
///*
  // Für die Simulation der mehreren Petrischalen wurde die kleine Bleiburg vom GEM-Detektor verwendet.
  // Für die experimentellen Messungen wurde aber die Bleiburg umgebaut zur großen vom EGPC. 

  G4double pos_alu = distance_source_detector + 112.46 + 5. + 25. - 400.3/2.; // mm

  // Aluplatten außen

  // links
  AluFront_out *alufront_out1 = new AluFront_out(world_logical);
  alufront_out1->Put(182.5, 0., pos_alu, 0., 0., 0.);
  // rechts
  AluFront_out *alufront_out2 = new AluFront_out(world_logical);
  alufront_out2->Put(-182.5, 0., pos_alu, 0., 0., 0.);
  // oben
  AluSide_out *aluside_out1 = new AluSide_out(world_logical);
  aluside_out1->Put(0., 185., pos_alu, 0., 0., 90. *deg);
  // unten
  AluSide_out *aluside_out2 = new AluSide_out(world_logical);
  aluside_out2->Put(0., -185., pos_alu, 0., 0., 90. *deg);

  // Aluplatten innen

  // links
  AluFront_in *alufront_in1 = new AluFront_in(world_logical);
  alufront_in1->Put(118.75, 0., pos_alu, 0., 0., 0.);
  // rechts
  AluFront_in *alufront_in2 = new AluFront_in(world_logical);
  alufront_in2->Put(-118.75, 0., pos_alu, 0., 0., 0.);
  // oben
  AluSide_in *aluside_in1 = new AluSide_in(world_logical);
  aluside_in1->Put(0., 118.75, pos_alu, 0., 0., 90. *deg);
  // unten
  AluSide_in *aluside_in2 = new AluSide_in(world_logical);
  aluside_in2->Put(0., -118.75, pos_alu, 0., 0., 90. *deg);

  // Deckelplatte
  //AluTop *alutop = new AluTop(world_logical);
  //alutop->Put(0., 0., distance_source_detector + 134. - (134 - {{end_cap_length_above_table}}) - 300. - 15./2., 0., 90. *deg, 0.);
  
  //Bodenplatte mit Loch
  AluBottom_withHole *alubottom_withhole = new AluBottom_withHole(world_logical);
  alubottom_withhole->Put(0., 0., distance_source_detector + 112.46 + 5 + 25./2. , 0., 0., 0.);

  //Bodenplatte mit Loch
  GEM_AluBottom_withHole *gem_alubottom_withhole = new GEM_AluBottom_withHole(world_logical);
  gem_alubottom_withhole->Put(0., 0., distance_source_detector + 112.46 + 5./2., 0., 0., 0.);

  // Bleiblöcke 1. Reihe

  G4double pos_brick = distance_source_detector + 112.46 + 5. + 25.;

  // Reihe oben
  NormBrick *normbrick_1o = new NormBrick(world_logical);
  normbrick_1o->Put(76.25, 151.875, pos_brick - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1o = new ShortNormBrick(world_logical);
  shortnormbrick_1o->Put(-75., 151.875, pos_brick - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  NormBrick *normbrick_1l = new NormBrick(world_logical);
  normbrick_1l->Put(150.625, 24.875, pos_brick - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  ShortNormBrick *shortnormbrick_1l = new ShortNormBrick(world_logical);
  shortnormbrick_1l->Put(150.625, -126.375, pos_brick - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Reihe unten
  NormBrick *normbrick_1u = new NormBrick(world_logical);
  normbrick_1u->Put(25., -151.875, pos_brick - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1u = new ShortNormBrick(world_logical);
  shortnormbrick_1u->Put(-126.25, -151.875, pos_brick - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  NormBrick *normbrick_1r = new NormBrick(world_logical);
  normbrick_1r->Put(-150.625, -24.375, pos_brick - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1r = new ShortNormBrick(world_logical);
  shortnormbrick_1r->Put(-150.625, 126.375, pos_brick - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 2. Reihe

  // Reihe oben
  NormBrick *normbrick_2o = new NormBrick(world_logical);
  normbrick_2o->Put(-76.25, 151.875, pos_brick - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2o = new ShortNormBrick(world_logical);
  shortnormbrick_2o->Put(75., 151.875, pos_brick - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  NormBrick *normbrick_2l = new NormBrick(world_logical);
  normbrick_2l->Put(150.625, -24.875, pos_brick - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2l = new ShortNormBrick(world_logical);
  shortnormbrick_2l->Put(150.625, 126.375, pos_brick - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Reihe unten
  NormBrick *normbrick_2u = new NormBrick(world_logical);
  normbrick_2u->Put(-25., -151.875, pos_brick - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2u = new ShortNormBrick(world_logical);
  shortnormbrick_2u->Put(126.25, -151.875, pos_brick - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  // Reihe rechts
  NormBrick *normbrick_2r = new NormBrick(world_logical);
  normbrick_2r->Put(-150.625, -74.375, pos_brick - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2r = new ShortNormBrick(world_logical);
  shortnormbrick_2r->Put(-150.625, 76.875, pos_brick - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 3. Reihe

  // Reihe oben
  NormBrick *normbrick_3o = new NormBrick(world_logical);
  normbrick_3o->Put(76.25, 151.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_3o = new ShortNormBrick(world_logical);
  shortnormbrick_3o->Put(-75., 151.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  ShortShortNormBrick *shortshortnormbrick_3o = new ShortShortNormBrick(world_logical);
  shortshortnormbrick_3o->Put(-151.25, 151.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  // Reihe links
  NormBrick *normbrick_3l = new NormBrick(world_logical);
  normbrick_3l->Put(150.625, 24.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  ShortNormBrick *shortnormbrick_3l = new ShortNormBrick(world_logical);
  shortnormbrick_3l->Put(150.625, -126.375, pos_brick - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Reihe unten
  NormBrick *normbrick_3u = new NormBrick(world_logical);
  normbrick_3u->Put(25., -151.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_3u = new ShortNormBrick(world_logical);
  shortnormbrick_3u->Put(-126.25, -151.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  NormBrick *normbrick_3r = new NormBrick(world_logical);
  normbrick_3r->Put(-150.625, 24.375, pos_brick - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortShortNormBrick *shortshortnormbrick_3r = new ShortShortNormBrick(world_logical);
  shortshortnormbrick_3r->Put(-150.625, -100.675, pos_brick - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 4. Reihe

  // Reihe oben
  NormBrick *normbrick_4o = new NormBrick(world_logical);
  normbrick_4o->Put(-76.25, 151.875, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4o = new ShortNormBrick(world_logical);
  shortnormbrick_4o->Put(75., 151.875, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  //ShortShortNormBrick *shortshortnormbrick_4o = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_4o->Put(151.25, 151.875, -115, 90. *deg, 90. *deg, 90. *deg);

  // Reihe links
  NormBrick *normbrick_4l = new NormBrick(world_logical);
  normbrick_4l->Put(150.625, -24.875, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4l = new ShortNormBrick(world_logical);
  shortnormbrick_4l->Put(150.625, 126.375, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  //ShortShortNormBrick *shortshortnormbrick_4l = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_4l->Put(150.625, -151.125, pos_brick - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Reihe unten
  NormBrick *normbrick_4u = new NormBrick(world_logical);
  normbrick_4u->Put(-25., -151.875, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4u = new ShortNormBrick(world_logical);
  shortnormbrick_4u->Put(126.25, -151.875, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  //ShortShortNormBrick *shortshortnormbrick_4u = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_4u->Put(101.25, -151.875, pos_brick - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Reihe rechts
  NormBrick *normbrick_4r = new NormBrick(world_logical);
  normbrick_4r->Put(-150.625, -74.375, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4r = new ShortNormBrick(world_logical);
  shortnormbrick_4r->Put(-150.625, 76.875, pos_brick - 100. - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

//*/

  /***************** Plastikbox für Kalibrierquelle P-1177 *********************************/

 // G4RotationMatrix *rottest = new G4RotationMatrix();
 //   rottest->rotateX(180. *deg);
 //   rottest->rotateY(90. *deg);
 //  rottest->rotateZ(0);

 // G4Box *plastikbox_solid = new G4Box("Plastikbox_Solid", 0.5 * 1.88 *mm, 0.5 * 10.95 *mm, 0.5 * 23.5 *mm);
 // G4LogicalVolume *plastikbox_logical = new G4LogicalVolume(plastikbox_solid, Epoxy, "Plastikbox_Logical");
 // G4PVPlacement * plastikbox_phys = new G4PVPlacement(rottest, G4ThreeVector(0, 0, -676.), plastikbox_logical, "Plastikbox", world_logical, false, 0);

  /***************** Filterhalter *********************************/

  //Filterhalter *filterhalter = new Filterhalter(world_logical);
  //filterhalter->Put(0., 0., -18., 180 * deg, 0., 0.);

  /***************** Testgeometrie *********************************/

  //G4String gdmlFileName = "../utr-radiationprotection/DetectorConstruction/Others/Efficiency/Volumes/testgeometrie.gdml";
  //G4GDMLParser parser;
  //parser.Read(gdmlFileName);
  //G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

  //G4LogicalVolume *testgeometrie_Logical = gdmlWorld->GetLogicalVolume();

  //G4Colour grey(0.5, 0.5, 0.5);
  //testgeometrie_Logical->SetVisAttributes(new G4VisAttributes(grey));

  //G4RotationMatrix *rottest = new G4RotationMatrix();
  //  rottest->rotateX(0);
  //  rottest->rotateY(180. *deg);
  //  rottest->rotateZ(0);

  //G4PVPlacement *testgeometrie = new G4PVPlacement(rottest, G4ThreeVector(0, 0, -794.76), testgeometrie_Logical,
  //                    "Testgeometrie", world_logical, false, 0);

  return world_physical;

}

// Definiere das Detektorvolumen als Detektor/sensitives Volumen in Geant4
void DetectorConstruction::ConstructSDandField() {

	// Use ParticleSD instead of EnergyDepositionSD, as ParticleSD records the hits of each particle within a event individually regardless whether the particle actually deposited energy in the detector or not.
	// An EnergyDepositionSD however only records a single particle per event and only if it actually left some energy in the detector
	EnergyDepositionSD *DetectorSD = new EnergyDepositionSD("Detector_logical", "Detector_logical");
	G4SDManager::GetSDMpointer()->AddNewDetector(DetectorSD);
	DetectorSD->SetDetectorID(0);
	SetSensitiveDetector("Detector_logical", DetectorSD, true);
}
