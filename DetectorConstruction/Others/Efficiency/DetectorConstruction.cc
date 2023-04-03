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
 * The simulation uses the ParticleSD detector type, which records particle information at any time when a particle propagates into a new target segment for the first time (or when it is created there).
 *
 */

G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Setup Properties *****************/

  const double distance_source_detector = 100. * mm;    // <------ adjust distance from source to detector
  const G4String filter_material_name = "G4_Pb";     // <------ adjust filter material
  const double filter_thickness = 0.5 * mm;           // <------ adjust filter thickness

  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  
  /***************** Detector Properties *****************/

  HPGe_Coaxial_Properties HPGe_jonny_props;

  HPGe_jonny_props.detector_radius = 0.5 * 63.2 * mm; // (A) in ORTEC data sheet
  HPGe_jonny_props.detector_length = 84.8 * mm; // (B) in ORTEC data sheet
  HPGe_jonny_props.detector_face_radius = 8. * mm; // (J) in ORTEC data sheet
  HPGe_jonny_props.hole_radius = 0.5 * 9.2 * mm; // (C) in ORTEC data sheet
  HPGe_jonny_props.hole_depth = 73.1 * mm; // (D) in ORTEC data sheet
  HPGe_jonny_props.mount_cup_thickness = 0.8 * mm; // (K) in ORTEC data sheet
  HPGe_jonny_props.mount_cup_base_thickness = 3. * mm; // ORTEC data sheet
  HPGe_jonny_props.mount_cup_material = "G4_Al"; // ORTEC data sheet
  HPGe_jonny_props.end_cap_outer_radius = 0.5 * 80. * mm; // Estimated
  HPGe_jonny_props.end_cap_to_crystal_gap_front = 4. * mm; // (G) in ORTEC data sheet
  HPGe_jonny_props.end_cap_thickness = 1. * mm; // (L) in ORTEC data sheet
  HPGe_jonny_props.end_cap_window_thickness = 0.5 * mm; // (I) in ORTEC data sheet
  HPGe_jonny_props.end_cap_length = 6.5 * inch; // Measured at the detector
  HPGe_jonny_props.end_cap_material = "G4_Al"; // ORTEC data sheet
  HPGe_jonny_props.end_cap_window_material = "G4_Be"; // ORTEC data sheet
  HPGe_jonny_props.cold_finger_radius = 0.5 * 4. * mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
  HPGe_jonny_props.cold_finger_material = "G4_Cu"; // Estimated
  HPGe_jonny_props.connection_length = 1.5 * inch; // Measured
  HPGe_jonny_props.connection_radius = 0.75 * inch; // Estimated
  HPGe_jonny_props.dewar_offset = 0. * inch; // Measured
  HPGe_jonny_props.connection_material = "G4_Al"; // Estimated
  HPGe_jonny_props.dewar_length = 12.5 * inch; // Measured
  HPGe_jonny_props.dewar_outer_radius = 4.5 * inch; // Measured
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

  const double world_x =  HPGe_jonny_props.dewar_outer_radius;
  const double world_y =  HPGe_jonny_props.dewar_outer_radius;
  const double world_z = 2 * HPGe_jonny_props.dewar_length + distance_source_detector;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, air, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);


  /******************** Detector ******************/

  HPGe_Coaxial *HPGe_jonny= new HPGe_Coaxial(world_logical, "Detector_logical");
  (*HPGe_jonny).setProperties(HPGe_jonny_props);
  //HPGe_jonny.useDewar();
  (*HPGe_jonny).Add_Filter(filter_material_name, filter_thickness, HPGe_jonny_props.detector_radius + 2 *HPGe_jonny_props.dewar_wall_thickness);
  (*HPGe_jonny).Construct(G4ThreeVector(0., 0., 0), 0, twopi, distance_source_detector, 0);


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
