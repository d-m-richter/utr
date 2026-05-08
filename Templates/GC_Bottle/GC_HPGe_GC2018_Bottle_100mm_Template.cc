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
#include "G4UnionSolid.hh"

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
//#include "Marinelli_Becher.hh"

// PlasticBottle
#include "PlasticBottle.hh"

// Quellenhalter
#include "Quellenhalter.hh"

// Hohlzylinder
#include "Hohlzylinder.hh"

// Filterhalter
#include "Filterhalter.hh"

// Input for petri dish
#include "Folienmethode.hh"

// Bleiburg
#include "Lead_Castle_for_GC2018.hh"

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
 * The detector is a p-type coaxial HPGe detector with a relative efficiency of 20 % at 1332 keV from Mirion.
 */

G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Setup Properties *****************/
  // There is no filter

  // In this case distance_source_detector describes the distance between end cap and zero point
  const double distance_source_detector = 0.; //{{distance_source_detector}}; // 100. * mm;    // <------ adjust distance from source to detector
//  const G4String filter_material_name = "G4_Pb";     // <------ adjust filter material
//  const double filter_thickness = 0.5 * mm;           // <------ adjust filter thickness

  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
 // G4Material *PLA = nist->FindOrBuildMaterial("G4_PLA");
  G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material *Ge = nist->FindOrBuildMaterial("G4_Ge");
  G4Material *Mylar = nist->FindOrBuildMaterial("G4_Mylar");
  G4Material *Li = nist->FindOrBuildMaterial("G4_Li");
  G4Material *B = nist->FindOrBuildMaterial("G4_B");

  //Epoxy from https://www.phenix.bnl.gov/~suhanov/ncc/geant/rad-source/src/ExN03DetectorConstruction.cc
  //G4double density = 1.2*g/cm3;
  //G4Material* Epoxy = new G4Material("Epoxy" , density, 4);
  //Epoxy->AddElement(nist->FindOrBuildElement("H"), 32);
  //Epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
  //Epoxy->AddElement(nist->FindOrBuildElement("N"), 2);
  //Epoxy->AddElement(nist->FindOrBuildElement("O"), 4);
  
  
  /***************** Detector Properties *****************/

  HPGe_Coaxial_Properties HPGe_GC2018_props;
  HPGe_GC2018_props.detector_radius = 0.5 * {{crystal_diameter}} * mm; // 0.5 * 60.6 mm with 0.2 mm uncertainty
  HPGe_GC2018_props.detector_length = {{crystal_length}} * mm; // 33.3 mm with 0.3 mm uncertainty 
  HPGe_GC2018_props.detector_face_radius = 0. * mm; // {{face_radius}} * mm; // 1 mm nominal radius
  HPGe_GC2018_props.detector_back_radius = 0. * mm; // {{back_radius}} * mm; // 1 mm nominal radius
  HPGe_GC2018_props.hole_radius = 0.5 * {{hole_diameter}} * mm; // 9.5 * mm; 
  HPGe_GC2018_props.hole_depth = {{hole_depth}} * mm; // 16. * mm;
  HPGe_GC2018_props.hole_face_radius = 0. * mm; // {{hole_face_radius}} // 0. * mm
  HPGe_GC2018_props.dead_layer_side = {{dead_layer_side}} * mm; // 0.6 * mm
  HPGe_GC2018_props.dead_layer_top = {{dead_layer_top}} * mm; // 0.6 * mm
  HPGe_GC2018_props.dead_layer_material = "G4_Ge"; 
  HPGe_GC2018_props.hole_dead_layer = 0.0003 * mm; // {{hole_dead_layer}} * mm; // 0.3 * micron Ge
  HPGe_GC2018_props.hole_dead_layer_material = "G4_Ge"; 
  HPGe_GC2018_props.mount_cup_thickness = 1.6 * mm; // 0.8 * mm Al
  HPGe_GC2018_props.mount_cup_base_thickness = 3.1 * mm; // 3 * mm Al
  HPGe_GC2018_props.mount_cup_material = "G4_Al";
  HPGe_GC2018_props.mount_cup_face_material = "G4_POLYETHYLENE"; // HD PE (high-density polythylene 0.961 g/cm^3 [Wikipedia])
  HPGe_GC2018_props.mount_cup_upper_window_thickness = 0.5 * mm; // {{mount_cup_upper_face_thickness}}; 
  HPGe_GC2018_props.mount_cup_upper_window_material = "G4_POLYETHYLENE"; // 1.0 * mm HD PE
  HPGe_GC2018_props.mount_cup_under_window_thickness = {{mount_cup_under_window_thickness}} * mm; //0.5 * mm; // {{mount_cup_under_face_thickness}};
  HPGe_GC2018_props.mount_cup_under_window_material = "G4_POLYETHYLENE"; // nothing
  HPGe_GC2018_props.mount_cup_face_thickness = HPGe_GC2018_props.mount_cup_upper_window_thickness + HPGe_GC2018_props.mount_cup_under_window_thickness;//1. * mm; // 1. * mm HD PE 
  HPGe_GC2018_props.end_cap_outer_radius = 0.5 * 76.2 * mm; // {{end_cap_radius}} * mm; // 76.2 * mm Al with 0.1 mm uncertainty
  HPGe_GC2018_props.end_cap_to_crystal_gap_front = {{end_cap_to_crystal_gap_front}} * mm; // 3.5 * mm with 0.5 mm uncertainty 
  HPGe_GC2018_props.end_cap_thickness = 1.5 * mm; // {{end_cap_thickness}} * mm; // 1.5 * mm Al with 0.1 mm uncertainty
  HPGe_GC2018_props.end_cap_window_thickness = {{end_cap_window_thickness}} * mm; // 1.5 * mm Al with 0.1 mm uncertainty
  HPGe_GC2018_props.end_cap_length = 110. * mm; // {{end_cap_stand_out}}; // 134 * mm  Al
  HPGe_GC2018_props.end_cap_material = "G4_Al"; // 
  HPGe_GC2018_props.end_cap_window_material = "G4_Al"; // 
  HPGe_GC2018_props.cold_finger_radius = 0.5 * 4. * mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
  HPGe_GC2018_props.cold_finger_material = "G4_Cu"; // Estimated
  HPGe_GC2018_props.connection_length = 1.5 * inch;
  HPGe_GC2018_props.connection_radius = 0.75 * inch; // Estimated
  HPGe_GC2018_props.dewar_offset = 0. * inch;
  HPGe_GC2018_props.connection_material = "G4_Al"; // Estimated
  HPGe_GC2018_props.dewar_length = 12.5 * inch;
  HPGe_GC2018_props.dewar_outer_radius = 4.5 * inch;
  HPGe_GC2018_props.dewar_wall_thickness = 5. * mm; // Estimated
  HPGe_GC2018_props.dewar_material = "G4_Al"; // Estimated
  HPGe_GC2018_props.hole_face_radius = HPGe_GC2018_props.hole_radius; // Estimated to be the same as hole radius
  HPGe_GC2018_props.cold_finger_penetration_depth = HPGe_GC2018_props.hole_depth - 5. * mm; // Estimated
  HPGe_GC2018_props.end_cap_to_crystal_gap_side =
      HPGe_GC2018_props.end_cap_outer_radius -
      HPGe_GC2018_props.end_cap_thickness -
      HPGe_GC2018_props.mount_cup_thickness -
      HPGe_GC2018_props.detector_radius -
      HPGe_GC2018_props.dead_layer_side; // Calculated from outer radius and other given dimensions
  HPGe_GC2018_props.mount_cup_length = 95.2 * mm; // {{mount_cup_length}} * mm; // 95.2 * mm Al
 //     HPGe_GC2018_props.end_cap_length -
 //     HPGe_GC2018_props.end_cap_thickness -
 //     HPGe_GC2018_props.end_cap_to_crystal_gap_front; // Calculated from end cap length


  /***************** World Volume *****************/

  const double world_x =  1. *m; //HPGe_GC2018_props.dewar_outer_radius;
  const double world_y =  1. *m; //HPGe_GC2018_props.dewar_outer_radius;
  const double world_z =  1. *m; //2 * HPGe_GC2018_props.dewar_length + distance_source_detector;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, air, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);


  /******************** Detector ******************/

  HPGe_Coaxial *HPGe_GC2018= new HPGe_Coaxial(world_logical, "Detector_logical");
  (*HPGe_GC2018).setProperties(HPGe_GC2018_props);
  //HPGe_GEM20P470.useDewar();
  //(*HPGe_GEM20P470).Add_Filter(filter_material_name, filter_thickness, HPGe_GC2018_props.detector_radius + 2 *HPGe_GC2018_props.dewar_wall_thickness);
  (*HPGe_GC2018).Construct(G4ThreeVector(0., 0., 0), 0, twopi, distance_source_detector, 0);

  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateX(180);
  rot->rotateY(180);
  rot->rotateZ(180);

  /***************** Flaschenhalterung *********************************/

  Bottle_Holder *plasticholder = new Bottle_Holder(world_logical);
  //plasticholder->Put(0., 0., 93.875 - 70. - 19.6 + 3.0 + 20., 180 * deg, 0., 0.);
  plasticholder->Put(0., 0., 93.875 - 82. + 20., 180 * deg, 0., 0.);

  /***************** PE-Flasche *********************************/

  Plastic_Bottle *plasticbottle = new Plastic_Bottle(world_logical);
  //plasticbottle->Put(0., 0., 93.875 - 70. - 19.6 + 3.0 - 22. - 10., 180 * deg, 0., 0.);
  plasticbottle->Put(0., 0., 93.875 - 82. - 22. - 10., 180 * deg, 0., 0.);

  Plastic_Bottle_Filled *plasticbottlefilled = new Plastic_Bottle_Filled(world_logical);
  //plasticbottlefilled->Put(0., 0., 93.875 - 70. - 19.6 + 3.0 - 22. - 10. - 1.25, 180 * deg, 0., 0.);
  //plasticbottlefilled->Put(0., 0., 93.875 - 82. - 22. - 10. - 1.25, 180 * deg, 0., 0.);
  plasticbottlefilled->Put(0., 0., 93.875 - 82. - 22. - 10. - 1.15, 180 * deg, 0., 0.);


  /***************** Hohlzylinder *********************************/
/*
  G4double hohlzylinder_position = {{hollow_cylinder_position}};
  Hohlzylinder *hohlzylinder = new Hohlzylinder(world_logical);
  hohlzylinder->Put({{horizontal_shift_x}}, {{horizontal_shift_y}}, hohlzylinder_position, 180. *deg, 0., 0.);
*/
// In dem Hohlzylinder sind die Höhen von dem 30mm und 40mm und 100mm hohen Hohlzylinder enthalten.
///*
  //G4Tubs *hohlzylinder_solid = new G4Tubs("Hohlzylinder_Solid", 0.5 * 81.0 *mm, 0.5 * 102.0 *mm, 0.5 * (70. + 19.6 - 3.0) *mm, 0. *deg, 360. *deg); // für die Simulation mit mehreren Petrischalen wurde 117.5 mm für den Durchmesser verwendet
  G4Tubs *hohlzylinder_solid = new G4Tubs("Hohlzylinder_Solid", 0.5 * 81.0 *mm, 0.5 * 102.0 *mm, 0.5 * (82.) *mm, 0. *deg, 360. *deg); // für die Simulation mit mehreren Petrischalen wurde 117.5 mm für den Durchmesser verwendet
  G4LogicalVolume *hohlzylinder_logical = new G4LogicalVolume(hohlzylinder_solid, polyethylene, "Hohlzylinder_Logical");
  //G4PVPlacement *hohlzylinder_phys = new G4PVPlacement(0, G4ThreeVector({{x_variation}}, {{y_variation}}, 93.875 - (30.0 + 40.0 + 100.0 + 6.5)/2.), hohlzylinder_logical, "Hohlzylinder", world_logical, false, 0);
  //G4PVPlacement *hohlzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 93.875 - (70. + 19.6 - 3.0)/2.), hohlzylinder_logical, "Hohlzylinder", world_logical, false, 0);
  G4PVPlacement *hohlzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 93.875 - (82.)/2.), hohlzylinder_logical, "Hohlzylinder", world_logical, false, 0);
//*/

  /***************** Bleiburg *********************************/

 G4double pos = distance_source_detector + 93.875 + 5.; // mm this is the surface of the table

///*
  // Deckelplatte
  GC_AluTop *alutop = new GC_AluTop(world_logical);
  alutop->Put(0., 0., pos - 100. - 100. - 100. - 25. - 10./2., 0., 90. *deg, 0.);
  
  //Bodenplatte mit Loch
  GC_AluBottom_withHole *alubottom_withhole = new GC_AluBottom_withHole(world_logical);
  alubottom_withhole->Put(0., 0., pos - 5./2., 0., 0., 0.);

  // Bleiblöcke 1. Reihe

  // Reihe unten
  GC_NormBrick *normbrick_11u = new GC_NormBrick(world_logical);
  normbrick_11u->Put(125., -200., pos - 100./2., 90. *deg, 90. *deg, 90. *deg);

  GC_NormBrick *normbrick_12u = new GC_NormBrick(world_logical);
  normbrick_12u->Put(-75., -200., pos - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  GC_NormBrick *normbrick_11l = new GC_NormBrick(world_logical);
  normbrick_11l->Put(-200., -125., pos - 100./2., 90. *deg, 0. *deg, 90. *deg);

  GC_NormBrick *normbrick_12l = new GC_NormBrick(world_logical);
  normbrick_12l->Put(-200., 75., pos - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  // Reihe oben
  GC_NormBrick *normbrick_11o = new GC_NormBrick(world_logical);
  normbrick_11o->Put(75., 200., pos - 100./2., 90. *deg, 90. *deg, 90. *deg);

  GC_NormBrick *normbrick_12o = new GC_NormBrick(world_logical);
  normbrick_12o->Put(-125., 200., pos - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  GC_NormBrick *normbrick_11r = new GC_NormBrick(world_logical);
  normbrick_11r->Put(200., -75, pos - 100./2., 90. *deg, 0. *deg, 90. *deg);

  GC_NormBrick *normbrick_12r = new GC_NormBrick(world_logical);
  normbrick_12r->Put(200., 125, pos - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 2. Reihe

  // Reihe unten
  GC_NormBrick *normbrick_21u = new GC_NormBrick(world_logical);
  normbrick_21u->Put(-125., -200., pos - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  GC_NormBrick *normbrick_22u = new GC_NormBrick(world_logical);
  normbrick_22u->Put(75., -200., pos - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  GC_NormBrick *normbrick_21l = new GC_NormBrick(world_logical);
  normbrick_21l->Put(-200., 125., pos - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  GC_NormBrick *normbrick_22l = new GC_NormBrick(world_logical);
  normbrick_22l->Put(-200., -75., pos - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  // Reihe oben
  GC_NormBrick *normbrick_21o = new GC_NormBrick(world_logical);
  normbrick_21o->Put(-75., 200., pos - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  GC_NormBrick *normbrick_22o = new GC_NormBrick(world_logical);
  normbrick_22o->Put(125., 200., pos - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  GC_NormBrick *normbrick_21r = new GC_NormBrick(world_logical);
  normbrick_21r->Put(200., 75, pos - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  GC_NormBrick *normbrick_22r = new GC_NormBrick(world_logical);
  normbrick_22r->Put(200., -125, pos - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 3. Reihe

  // Reihe unten
  GC_NormBrick *normbrick_31u = new GC_NormBrick(world_logical);
  normbrick_31u->Put(125., -200., pos - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  GC_NormBrick *normbrick_32u = new GC_NormBrick(world_logical);
  normbrick_32u->Put(-75., -200., pos - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  GC_NormBrick *normbrick_31l = new GC_NormBrick(world_logical);
  normbrick_31l->Put(-200., -125., pos - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  GC_NormBrick *normbrick_32l = new GC_NormBrick(world_logical);
  normbrick_32l->Put(-200., 75., pos - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  // Reihe oben
  GC_NormBrick *normbrick_31o = new GC_NormBrick(world_logical);
  normbrick_31o->Put(75., 200., pos - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  GC_NormBrick *normbrick_32o = new GC_NormBrick(world_logical);
  normbrick_32o->Put(-125., 200., pos - 100. - 100. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  GC_NormBrick *normbrick_31r = new GC_NormBrick(world_logical);
  normbrick_31r->Put(200., -75, pos - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  GC_NormBrick *normbrick_32r = new GC_NormBrick(world_logical);
  normbrick_32r->Put(200., 125, pos - 100. - 100. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 4. Reihe mit SlimNormBricks

  // Reihe unten
  GC_SlimNormBrick *slimnormbrick_41u = new GC_SlimNormBrick(world_logical);
  slimnormbrick_41u->Put(150., -200., pos - 100. - 100. - 100. - 25./2., 0. *deg, 90. *deg, 90. *deg);

  GC_SlimNormBrick *slimnormbrick_42u = new GC_SlimNormBrick(world_logical);
  slimnormbrick_42u->Put(-50., -200., pos - 100. - 100. - 100. - 25./2., 0. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  GC_SlimNormBrick *slimnormbrick_41l = new GC_SlimNormBrick(world_logical);
  slimnormbrick_41l->Put(-200., -150., pos - 100. - 100. - 100. - 25./2., 90. *deg, 0. *deg, 0. *deg);

  GC_SlimNormBrick *slimnormbrick_42l = new GC_SlimNormBrick(world_logical);
  slimnormbrick_42l->Put(-200., 50., pos - 100. - 100. - 100. - 25./2., 90. *deg, 0. *deg, 0. *deg);
  
  // Reihe oben
  GC_SlimNormBrick *slimnormbrick_41o = new GC_SlimNormBrick(world_logical);
  slimnormbrick_41o->Put(-150., 200., pos - 100. - 100. - 100. - 25./2., 0. *deg, 90. *deg, 90. *deg);

  GC_SlimNormBrick *slimnormbrick_42o = new GC_SlimNormBrick(world_logical);
  slimnormbrick_42o->Put(50., 200., pos - 100. - 100. - 100. - 25./2., 0. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  GC_SlimNormBrick *slimnormbrick_41r = new GC_SlimNormBrick(world_logical);
  slimnormbrick_41r->Put(200., 150., pos - 100. - 100. - 100. - 25./2., 90. *deg, 0. *deg, 0. *deg);

  GC_SlimNormBrick *slimnormbrick_42r = new GC_SlimNormBrick(world_logical);
  slimnormbrick_42r->Put(200., -50., pos - 100. - 100. - 100. - 25./2., 90. *deg, 0. *deg, 0. *deg);
  
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
