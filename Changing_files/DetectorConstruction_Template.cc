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
#include "Rundflasche.hh"

// Quellenhalter
#include "Quellenhalter.hh"

// Hohlzylinder
#include "Hohlzylinder.hh"

// Filterhalter
#include "Filterhalter.hh"

// Bleiburg
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
 */

G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Setup Properties *****************/
  // There is no filter

  // In this case distance_source_detector describes the distance between end cap and zero point
  const double distance_source_detector = 100. * mm;    // <------ adjust distance from source to detector
 // const double distance_adapter_detector = 10. * mm;
//  const G4String filter_material_name = "G4_Pb";     // <------ adjust filter material
//  const double filter_thickness = 0.5 * mm;           // <------ adjust filter thickness

  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
 // G4Material *PLA = nist->FindOrBuildMaterial("G4_PLA");
  //G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

  //Epoxy from https://www.phenix.bnl.gov/~suhanov/ncc/geant/rad-source/src/ExN03DetectorConstruction.cc
  G4double density = 1.2*g/cm3;
  G4Material* Epoxy = new G4Material("Epoxy" , density, 4);
  Epoxy->AddElement(nist->FindOrBuildElement("H"), 32);
  Epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
  Epoxy->AddElement(nist->FindOrBuildElement("N"), 2);
  Epoxy->AddElement(nist->FindOrBuildElement("O"), 4);
  
  
  /***************** Detector Properties *****************/

  HPGe_Coaxial_Properties HPGe_jonny_props;
  // Volume of Ge Crystal = 166 * cm3
  // Dead layer of Ge Crystal = 1mmGe
  HPGe_jonny_props.detector_radius = 0.5 * {{cystal_diameter}} * mm; // 64 mm in EurisysMesure data sheet
  HPGe_jonny_props.detector_length = {{crystal_length}} * mm; // 53 mm in EurisysMesure data sheet
  HPGe_jonny_props.detector_face_radius = 0. * mm;
  HPGe_jonny_props.hole_radius = 0.5 * {{hole_diameter}} * mm; // 11.35 * mm; 
  HPGe_jonny_props.hole_depth = {{hole_depth}} * mm; // 44.5 * mm;
  HPGe_jonny_props.dead_layer_side = {{dead_layer_side}} * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.dead_layer_top = {{dead_layer_top}} * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.mount_cup_thickness = 0.8 * mm;
  HPGe_jonny_props.mount_cup_base_thickness = 3. * mm;
  HPGe_jonny_props.mount_cup_material = "G4_Al";
  HPGe_jonny_props.end_cap_outer_radius = 0.5 * 80. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_to_crystal_gap_front = {{distance_detector_end_cap}} * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_thickness = 1. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_window_thickness = {{end_cap_face_thickness}} * mm;
  HPGe_jonny_props.end_cap_length = {{end_cap_stand_out}} + 3 *mm; // gemessen 132. * mm; // 135 mm in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_material = "G4_Al"; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_window_material = "G4_Al"; // in EurisysMesure data sheet
  HPGe_jonny_props.cold_finger_radius = 0.5 * 4. * mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
  HPGe_jonny_props.cold_finger_material = "G4_Cu"; // Estimated
  HPGe_jonny_props.connection_length = 1.5 * inch;
  HPGe_jonny_props.connection_radius = 0.75 * inch; // Estimated
  HPGe_jonny_props.dewar_offset = 0. * inch;
  HPGe_jonny_props.connection_material = "G4_Al"; // Estimated
  HPGe_jonny_props.dewar_length = 12.5 * inch;
  HPGe_jonny_props.dewar_outer_radius = 4.5 * inch;
  HPGe_jonny_props.dewar_wall_thickness = 5. * mm; // Estimated
  HPGe_jonny_props.dewar_material = "G4_Al"; // Estimated
  HPGe_jonny_props.hole_face_radius = HPGe_jonny_props.hole_radius; // Estimated to be the same as hole radius
  HPGe_jonny_props.cold_finger_penetration_depth = HPGe_jonny_props.hole_depth - 5. * mm; // Estimated
  HPGe_jonny_props.end_cap_to_crystal_gap_side =
      HPGe_jonny_props.end_cap_outer_radius -
      HPGe_jonny_props.end_cap_thickness -
      HPGe_jonny_props.mount_cup_thickness -
      HPGe_jonny_props.detector_radius -
      HPGe_jonny_props.dead_layer_side; // Calculated from outer radius and other given dimensions
  HPGe_jonny_props.mount_cup_length =
      HPGe_jonny_props.end_cap_length -
      HPGe_jonny_props.end_cap_window_thickness -
      HPGe_jonny_props.end_cap_to_crystal_gap_front; // Calculated from end cap length


  /***************** World Volume *****************/

  const double world_x =  1. *m; //HPGe_jonny_props.dewar_outer_radius;
  const double world_y =  1. *m; //HPGe_jonny_props.dewar_outer_radius;
  const double world_z =  1. *m; //2 * HPGe_jonny_props.dewar_length + distance_source_detector;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, air, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);


  /******************** Detector ******************/

  HPGe_Coaxial *HPGe_jonny= new HPGe_Coaxial(world_logical, "Detector_logical");
  (*HPGe_jonny).setProperties(HPGe_jonny_props);
  //HPGe_jonny.useDewar();
  //(*HPGe_jonny).Add_Filter(filter_material_name, filter_thickness, HPGe_jonny_props.detector_radius + 2 *HPGe_jonny_props.dewar_wall_thickness);
  (*HPGe_jonny).Construct(G4ThreeVector(0., 0., 0), 0, twopi, distance_source_detector, 0);

  /******************** Adapter ******************/

  //Adapter *adapter = new Adapter(world_logical);
  //adapter->Put(0., 0., 70., 180 * deg, 0., 0.);

  /***************** ProbenhalterLang ************/

  //ProbenhalterLang *probenhalterLang = new ProbenhalterLang(world_logical);
  //probenhalterLang->Put(0., 0., 70., 180 * deg, 0., 0.);
  
  /***************** ProbenhalterKurz ************/

  //ProbenhalterKurz *probenhalterKurz = new ProbenhalterKurz(world_logical);
  //probenhalterKurz->Put(0., 0., 70., 180 * deg, 0., 0.);

  /***************** Marinelli-Becher *********************************/

  //MarinelliBeaker *marinelliBeaker = new MarinelliBeaker(world_logical);
  //marinelliBeaker->Put(0., 0., 110.);

  /***************** Marinelli-Becher Deckel *********************************/

  //MarinelliDeckel *marinelliDeckel = new MarinelliDeckel(world_logical);
  //marinelliDeckel->Put(0., 0., 18.);

  /***************** Rundflasche *********************************/

  //Rundflasche *rundflasche = new Rundflasche(world_logical);
  //rundflasche->Put(0., 0., -18., 180 * deg, 0., 0.);

  /***************** Quellenhalter *********************************/

  G4double hohlzylinder_position = 232.;
  G4double quellenhalter_position = hohlzylinder_position - {{hollow_cylinder}};

  Quellenhalter *quellenhalter = new Quellenhalter(world_logical);
  quellenhalter->Put(0., 0., quellenhalter_position, 180. *deg, 0., 0.);

  /***************** Hohlzylinder *********************************/

  Hohlzylinder *hohlzylinder = new Hohlzylinder(world_logical);
  hohlzylinder->Put(0., 0., hohlzylinder_position, 180. *deg, 0., 0.);

  /***************** Plastikzylinder für Kalibrierquelle P-1030 *********************************/

  G4double plastikzylinder_position = {{source_placement}} * mm;

  G4Tubs *plastikzylinder_solid = new G4Tubs("Plastikzylinder_Solid", 0 *mm, 0.5 * 25.03 *mm, 0.5 * {{source_thickness}} *mm, 0. *deg, 360. *deg);
  G4LogicalVolume *plastikzylinder_logical = new G4LogicalVolume(plastikzylinder_solid, Epoxy, "Plastikzylinder_Logical");
  G4PVPlacement *plastikzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, plastikzylinder_position), plastikzylinder_logical, "Plastikzylinder", world_logical, false, 0);

  /***************** Bleiburg *********************************/

  // Aluplatten außen

  // links
  AluFront_out *alufront_out1 = new AluFront_out(world_logical);
  alufront_out1->Put(182.5, 0., 100. + 132. + 25. - 400.3/2., 0., 0., 0.);
  // rechts
  AluFront_out *alufront_out2 = new AluFront_out(world_logical);
  alufront_out2->Put(-182.5, 0., 100. + 132. + 25. - 400.3/2., 0., 0., 0.);
  // oben
  AluSide_out *aluside_out1 = new AluSide_out(world_logical);
  aluside_out1->Put(0., 185., 100. + 132. + 25. - 400.3/2., 0., 0., 90. *deg);
  // unten
  AluSide_out *aluside_out2 = new AluSide_out(world_logical);
  aluside_out2->Put(0., -185., 100. + 132. + 25. - 400.3/2., 0., 0., 90. *deg);

  // Aluplatten innen

  // links
  AluFront_in *alufront_in1 = new AluFront_in(world_logical);
  alufront_in1->Put(118.75, 0., 100. + 132. + 25. - 400.3/2., 0., 0., 0.);
  // rechts
  AluFront_in *alufront_in2 = new AluFront_in(world_logical);
  alufront_in2->Put(-118.75, 0., 100. + 132. + 25. - 400.3/2., 0., 0., 0.);
  // oben
  AluSide_in *aluside_in1 = new AluSide_in(world_logical);
  aluside_in1->Put(0., 118.75, 100. + 132. + 25. - 400.3/2., 0., 0., 90. *deg);
  // unten
  AluSide_in *aluside_in2 = new AluSide_in(world_logical);
  aluside_in2->Put(0., -118.75, 100. + 132. + 25. - 400.3/2., 0., 0., 90. *deg);

  // Deckelplatte
  AluTop *alutop = new AluTop(world_logical);
  alutop->Put(0., 0., 100. + 132. + 25. - 400.3 - 10./2., 0., 90. *deg, 0.);
  
  //Bodenplatte mit Loch
  AluBottom_withHole *alubottom_withhole = new AluBottom_withHole(world_logical);
  alubottom_withhole->Put(0., 0., 100. + 132. + 25. - 25./2., 0., 0., 0.);

  // Bleiblöcke 1. Reihe

  // Reihe oben
  NormBrick *normbrick_1o = new NormBrick(world_logical);
  normbrick_1o->Put(76.25, 151.875, 100. + 132. + 25. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1o = new ShortNormBrick(world_logical);
  shortnormbrick_1o->Put(-75., 151.875, 100. + 132. + 25. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  //ShortShortNormBrick *shortshortnormbrick_1o = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_1o->Put(-151.25, 151.875, 185, 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  NormBrick *normbrick_1l = new NormBrick(world_logical);
  normbrick_1l->Put(150.625, 24.875, 100. + 132. + 25. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1l = new ShortNormBrick(world_logical);
  shortnormbrick_1l->Put(150.625, -126.375, 100. + 132. + 25. - 100./2., 90. *deg, 0. *deg, 90. *deg);
  
  // Reihe unten
  NormBrick *normbrick_1u = new NormBrick(world_logical);
  normbrick_1u->Put(25., -151.875, 100. + 132. + 25. - 100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1u = new ShortNormBrick(world_logical);
  shortnormbrick_1u->Put(-126.25, -151.875, 100. + 132. + 25. - 100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  NormBrick *normbrick_1r = new NormBrick(world_logical);
  normbrick_1r->Put(-150.625, -24.375, 100. + 132. + 25. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_1r = new ShortNormBrick(world_logical);
  shortnormbrick_1r->Put(-150.625, 126.375, 100. + 132. + 25. - 100./2., 90. *deg, 0. *deg, 90. *deg);

  //ShortShortNormBrick *shortshortnormbrick_1r = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_1r->Put(-150.625, -100.675, 185, 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 2. Reihe

  // Reihe oben
  NormBrick *normbrick_2o = new NormBrick(world_logical);
  normbrick_2o->Put(-76.25, 151.875, 100. + 132. + 25. - 100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2o = new ShortNormBrick(world_logical);
  shortnormbrick_2o->Put(75., 151.875, 100. + 132. + 25. - 100. -100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  NormBrick *normbrick_2l = new NormBrick(world_logical);
  normbrick_2l->Put(150.625, -24.875, 100. + 132. + 25. - 100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2l = new ShortNormBrick(world_logical);
  shortnormbrick_2l->Put(150.625, 126.375, 100. + 132. + 25. - 100. -100./2., 90. *deg, 0. *deg, 90. *deg);
  
  //ShortShortNormBrick *shortshortnormbrick_2l = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_2l->Put(150.625, -151.125, 85, 90. *deg, 90. *deg, 90. *deg);

  // Reihe unten
  NormBrick *normbrick_2u = new NormBrick(world_logical);
  normbrick_2u->Put(-25., -151.875, 100. + 132. + 25. - 100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2u = new ShortNormBrick(world_logical);
  shortnormbrick_2u->Put(126.25, -151.875, 100. + 132. + 25. - 100. -100./2., 90. *deg, 90. *deg, 90. *deg);
  
  //ShortShortNormBrick *shortshortnormbrick_2u = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_2u->Put(101.25, -151.875, 85, 90. *deg, 90. *deg, 90. *deg);

  // Reihe rechts
  NormBrick *normbrick_2r = new NormBrick(world_logical);
  normbrick_2r->Put(-150.625, -74.375, 100. + 132. + 25. - 100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_2r = new ShortNormBrick(world_logical);
  shortnormbrick_2r->Put(-150.625, 76.875, 100. + 132. + 25. - 100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 3. Reihe

  // Reihe oben
  NormBrick *normbrick_3o = new NormBrick(world_logical);
  normbrick_3o->Put(76.25, 151.875, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_3o = new ShortNormBrick(world_logical);
  shortnormbrick_3o->Put(-75., 151.875, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortShortNormBrick *shortshortnormbrick_3o = new ShortShortNormBrick(world_logical);
  shortshortnormbrick_3o->Put(-151.25, 151.875, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe links
  NormBrick *normbrick_3l = new NormBrick(world_logical);
  normbrick_3l->Put(150.625, 24.875, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_3l = new ShortNormBrick(world_logical);
  shortnormbrick_3l->Put(150.625, -126.375, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);
  
  // Reihe unten
  NormBrick *normbrick_3u = new NormBrick(world_logical);
  normbrick_3u->Put(25., -151.875, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_3u = new ShortNormBrick(world_logical);
  shortnormbrick_3u->Put(-126.25, -151.875, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);
  
  // Reihe rechts
  NormBrick *normbrick_3r = new NormBrick(world_logical);
  normbrick_3r->Put(-150.625, 24.375, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortShortNormBrick *shortshortnormbrick_3r = new ShortShortNormBrick(world_logical);
  shortshortnormbrick_3r->Put(-150.625, -100.675, 100. + 132. + 25. - 100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  // Bleiblöcke 4. Reihe

  // Reihe oben
  NormBrick *normbrick_4o = new NormBrick(world_logical);
  normbrick_4o->Put(-76.25, 151.875, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4o = new ShortNormBrick(world_logical);
  shortnormbrick_4o->Put(75., 151.875, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);
  
  //ShortShortNormBrick *shortshortnormbrick_4o = new ShortShortNormBrick(world_logical);
  //shortshortnormbrick_4o->Put(151.25, 151.875, -115, 90. *deg, 90. *deg, 90. *deg);

  // Reihe links
  NormBrick *normbrick_4l = new NormBrick(world_logical);
  normbrick_4l->Put(150.625, -24.875, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4l = new ShortNormBrick(world_logical);
  shortnormbrick_4l->Put(150.625, 126.375, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);
  
  ShortShortNormBrick *shortshortnormbrick_4l = new ShortShortNormBrick(world_logical);
  shortshortnormbrick_4l->Put(150.625, -151.125, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  // Reihe unten
  NormBrick *normbrick_4u = new NormBrick(world_logical);
  normbrick_4u->Put(-25., -151.875, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  //ShortNormBrick *shortnormbrick_4u = new ShortNormBrick(world_logical);
  //shortnormbrick_4u->Put(126.25, -151.875, -115, 90. *deg, 90. *deg, 90. *deg);
  
  ShortShortNormBrick *shortshortnormbrick_4u = new ShortShortNormBrick(world_logical);
  shortshortnormbrick_4u->Put(101.25, -151.875, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 90. *deg, 90. *deg);

  // Reihe rechts
  NormBrick *normbrick_4r = new NormBrick(world_logical);
  normbrick_4r->Put(-150.625, -74.375, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);

  ShortNormBrick *shortnormbrick_4r = new ShortNormBrick(world_logical);
  shortnormbrick_4r->Put(-150.625, 76.875, 100. + 132. + 25. - 100. -100. -100. -100./2., 90. *deg, 0. *deg, 90. *deg);


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

  //G4String gdmlFileName = "../utr-jonny/DetectorConstruction/Others/Efficiency/Volumes/testgeometrie.gdml";
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
