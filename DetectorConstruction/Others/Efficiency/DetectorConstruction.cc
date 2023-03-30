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

#include "HPGe_Coaxial_Properties.hh"    // <- ?
#include "Units.hh"                      // <- ?


// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


// #include "G4UnitsTable.hh"
#include "utrConfig.h"
#include <iostream>
#include <array>


/*
 * Simple geometry to determine the efficiency of a detector implemented in this DetectorConstruction.
 * 
 * The simulation uses the ParticleSD detector type, which records particle information at any time when a
 * particle propagates into a new target segment for the first time (or when it is created there).
 *
 */


G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Setup Properties *****************/

  const double distance_source_detector = 10. * mm;           // <------ adjust distance from source to detector
  
  /***************** Detector Properties *****************/

  HPGe_Coaxial_Properties HPGe_jonny;

  HPGe_jonny.detector_radius = 0.5 * 63.2 * mm; // (A) in ORTEC data sheet
  HPGe_jonny.detector_length = 84.8 * mm; // (B) in ORTEC data sheet
  HPGe_jonny.detector_face_radius = 8. * mm; // (J) in ORTEC data sheet
  HPGe_jonny.hole_radius = 0.5 * 9.2 * mm; // (C) in ORTEC data sheet
  HPGe_jonny.hole_depth = 73.1 * mm; // (D) in ORTEC data sheet
  HPGe_jonny.mount_cup_thickness = 0.8 * mm; // (K) in ORTEC data sheet
  HPGe_jonny.mount_cup_base_thickness = 3. * mm; // ORTEC data sheet
  HPGe_jonny.mount_cup_material = "G4_Al"; // ORTEC data sheet
  HPGe_jonny.end_cap_outer_radius = 0.5 * 80. * mm; // Estimated
  HPGe_jonny.end_cap_to_crystal_gap_front = 4. * mm; // (G) in ORTEC data sheet
  HPGe_jonny.end_cap_thickness = 1. * mm; // (L) in ORTEC data sheet
  HPGe_jonny.end_cap_window_thickness = 0.5 * mm; // (I) in ORTEC data sheet
  HPGe_jonny.end_cap_length = 6.5 * inch; // Measured at the detector
  HPGe_jonny.end_cap_material = "G4_Al"; // ORTEC data sheet
  HPGe_jonny.end_cap_window_material = "G4_Be"; // ORTEC data sheet
  HPGe_jonny.cold_finger_radius = 0.5 * 4. * mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
  HPGe_jonny.cold_finger_material = "G4_Cu"; // Estimated
  HPGe_jonny.connection_length = 1.5 * inch; // Measured
  HPGe_jonny.connection_radius = 0.75 * inch; // Estimated
  HPGe_jonny.dewar_offset = 0. * inch; // Measured
  HPGe_jonny.connection_material = "G4_Al"; // Estimated
  HPGe_jonny.dewar_length = 12.5 * inch; // Measured
  HPGe_jonny.dewar_outer_radius = 4.5 * inch; // Measured
  HPGe_jonny.dewar_wall_thickness = 5. * mm; // Estimated
  HPGe_jonny.dewar_material = "G4_Al"; // Estimated
  HPGe_jonny.hole_face_radius = HPGe_jonny.hole_radius; // Estimated to be the same as hole radius
  HPGe_jonny.cold_finger_penetration_depth = HPGe_jonny.hole_depth - 5. * mm; // Estimated
  HPGe_jonny.end_cap_to_crystal_gap_side =
      HPGe_jonny.end_cap_outer_radius -
      HPGe_jonny.end_cap_thickness -
      HPGe_jonny.mount_cup_thickness -
      HPGe_jonny.detector_radius; // Calculated from outer radius and other given dimensions
  HPGe_jonny.mount_cup_length =
      HPGe_jonny.end_cap_length -
      HPGe_jonny.end_cap_window_thickness -
      HPGe_jonny.end_cap_to_crystal_gap_front; // Calculated from end cap length


  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

  /***************** World Volume *****************/

  const double world_x =  HPGe_jonny.detector_radius;
  const double world_y =  HPGe_jonny.detector_radius;
  const double world_z = 2 * HPGe_jonny.detector_length + distance_source_detector;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, air, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);

  /******************** Detector ******************/



  

	G4Tubs *Detector_solid = new G4Tubs("Detector_solid", 0, 10. *mm, HPGe_jonny.detector_length/2, 0, twopi);
	G4LogicalVolume *Detector_logical = new G4LogicalVolume(Detector_solid, air, "Detector_logical", 0, 0, 0);

	//Visualisierung (Farbe)
	Detector_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
	new G4PVPlacement(0, G4ThreeVector(0, 0, distance_source_detector + HPGe_jonny.detector_length/2), Detector_logical, "Detector", world_logical, false, 0);
	




  return world_physical;
}

// Definiere das Detektorvolumen als Detektor/sensitives Volumen in Geant4
void DetectorConstruction::ConstructSDandField() {

	// Use ParticleSD instead of EnergyDepositionSD, as ParticleSD records the hits of each particle within a event individually regardless whether the particle actually deposited energy in the detector or not.
	// An EnergyDepositionSD however only records a single particle per event and only if it actually left some energy in the detector
	ParticleSD *DetectorSD = new ParticleSD("Detector_logical", "Detector_logical");
	G4SDManager::GetSDMpointer()->AddNewDetector(DetectorSD);
	DetectorSD->SetDetectorID(0);
	SetSensitiveDetector("Detector_logical", DetectorSD, true);
}
