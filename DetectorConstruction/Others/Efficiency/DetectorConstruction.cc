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

  
  /***************** Detector Properties *****************/

  HPGe_Coaxial_Properties HPGe_jonny_props;
  // Volume of Ge Crystal = 166 * cm3
  // Dead layer of Ge Crystal = 1mmGe
  HPGe_jonny_props.detector_radius = 0.5 * 64. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.detector_length = 53. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.detector_face_radius = 0. * mm;
  HPGe_jonny_props.hole_radius = 0.5 * 18.8  * mm; // 11.35 * mm; 
  HPGe_jonny_props.hole_depth = 33.1 * mm; // 44.5 * mm;
  HPGe_jonny_props.mount_cup_thickness = 0.8 * mm;
  HPGe_jonny_props.mount_cup_base_thickness = 3. * mm;
  HPGe_jonny_props.mount_cup_material = "G4_Al";
  HPGe_jonny_props.end_cap_outer_radius = 0.5 * 80. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_to_crystal_gap_front = 4. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_thickness = 1. * mm; // in EurisysMesure data sheet
  HPGe_jonny_props.end_cap_window_thickness = 0.5 * mm;
  HPGe_jonny_props.end_cap_length = 135. * mm; // in EurisysMesure data sheet
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
      HPGe_jonny_props.detector_radius; // Calculated from outer radius and other given dimensions
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

  Quellenhalter *quellenhalter = new Quellenhalter(world_logical);
  quellenhalter->Put(0., 0., 109.93, 180 * deg, 0., 0.);

  /***************** Hohlzylinder *********************************/

  Hohlzylinder *hohlzylinder = new Hohlzylinder(world_logical);
  hohlzylinder->Put(0., 0., 205.5, 180 * deg, 0., 0.);

  /***************** Plastikzylinder für Kalibrierquelle P-1030 *********************************/

  //Epoxy from https://www.phenix.bnl.gov/~suhanov/ncc/geant/rad-source/src/ExN03DetectorConstruction.cc
  G4double density = 1.2*g/cm3;
  G4Material* Epoxy = new G4Material("Epoxy" , density, 4);
  Epoxy->AddElement(nist->FindOrBuildElement("H"), 32);
  Epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
  Epoxy->AddElement(nist->FindOrBuildElement("N"), 2);
  Epoxy->AddElement(nist->FindOrBuildElement("O"), 4);

  // Plastikzylinder aus Epocy um die Cs-137 Quelle herum
  G4Tubs *plastikzylinder_solid = new G4Tubs("Plastikzylinder_Solid", 0 *mm, 0.5 * 25.03 *mm, 0.5 * 2.94 *mm, 0. *deg, 360. *deg);
  G4LogicalVolume *plastikzylinder_logical = new G4LogicalVolume(plastikzylinder_solid, Epoxy, "Plastikzylinder_Logical");
  G4PVPlacement *plastikzylinder_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, -9.005), plastikzylinder_logical, "Plastikzylinder", world_logical, false, 0);


  /***************** Plastikbox für Kalibrierquelle P-1177 *********************************/

  //Epoxy from https://www.phenix.bnl.gov/~suhanov/ncc/geant/rad-source/src/ExN03DetectorConstruction.cc
 // G4double density = 1.2*g/cm3;
 // G4Material* Epoxy = new G4Material("Epoxy" , density, 4);
 // Epoxy->AddElement(nist->FindOrBuildElement("H"), 32);
 // Epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
 // Epoxy->AddElement(nist->FindOrBuildElement("N"), 2);
 // Epoxy->AddElement(nist->FindOrBuildElement("O"), 4);

 // G4RotationMatrix *rottest = new G4RotationMatrix();
 //   rottest->rotateX(180. *deg);
 //   rottest->rotateY(90. *deg);
 //  rottest->rotateZ(0);

  // Plastikbox aus Epocy um die Cs-137 Quelle herum
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
