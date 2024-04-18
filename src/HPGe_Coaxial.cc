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

#include <sstream>

#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

#include "Filter_Case.hh"
#include "HPGe_Coaxial.hh"
#include "OptimizePolycone.hh"

using std::stringstream;

void HPGe_Coaxial::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, G4double intrinsic_rotation_angle) const {

  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector symmetry_axis(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)); // Symmetry axis along which the single elements of the detector are constructed
  G4ThreeVector symmetry_axis_orthogonal(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta)); // Vector which is orthogonal to the symmetry axes. Needed for the construction of off-axis elements. As an arbitrary choice, take the unit vector with respect to theta in spherical coordinates.

  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);
  if (intrinsic_rotation_angle != 0.) {
    rotation->rotateZ(intrinsic_rotation_angle);
  }

  /************* End cap *************/
  // End cap side
  G4double end_cap_inner_radius = properties.detector_radius + properties.dead_layer_side + properties.mount_cup_thickness + properties.end_cap_to_crystal_gap_side;
  G4double end_cap_outer_radius = properties.detector_radius + properties.dead_layer_side + properties.mount_cup_thickness + properties.end_cap_to_crystal_gap_side + properties.end_cap_thickness;
  G4double end_cap_side_length = properties.end_cap_length - properties.end_cap_thickness; //properties.mount_cup_length + properties.end_cap_to_crystal_gap_front;

  G4Tubs *end_cap_side_solid = new G4Tubs(detector_name + "_end_cap_side_solid", end_cap_inner_radius, end_cap_outer_radius, properties.end_cap_length * 0.5, 0., twopi);
  G4LogicalVolume *end_cap_side_logical = new G4LogicalVolume(end_cap_side_solid, nist->FindOrBuildMaterial(properties.end_cap_material), detector_name + "_end_cap_side_logical");
  end_cap_side_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(rotation, global_coordinates + (dist_from_center + properties.end_cap_length * 0.5) * symmetry_axis, end_cap_side_logical, detector_name + "_end_cap_side", world_Logical, 0, 0, false);
///*
  // End cap window
  G4Tubs *end_cap_window_solid = new G4Tubs(detector_name + "_end_cap_window_solid", 0., end_cap_inner_radius, properties.end_cap_window_thickness * 0.5, 0., twopi);
  G4LogicalVolume *end_cap_window_logical = new G4LogicalVolume(end_cap_window_solid, nist->FindOrBuildMaterial(properties.end_cap_window_material), detector_name + "_end_cap_window_logical");
  end_cap_window_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(rotation, global_coordinates + (dist_from_center + properties.end_cap_thickness - properties.end_cap_window_thickness * 0.5) * symmetry_axis, end_cap_window_logical, detector_name + "_end_cap_window", world_Logical, 0, 0, false);
//*/
  // Vacuum inside end cap
  G4Tubs *end_cap_vacuum_solid = new G4Tubs(detector_name + "_end_cap_vacuum_solid", 0., end_cap_inner_radius, end_cap_side_length * 0.5, 0., twopi);
  G4LogicalVolume *end_cap_vacuum_logical = new G4LogicalVolume(end_cap_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"), detector_name + "_end_cap_vacuum_logical");
  end_cap_vacuum_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
  new G4PVPlacement(rotation, global_coordinates + (dist_from_center + properties.end_cap_thickness + end_cap_side_length * 0.5) * symmetry_axis, end_cap_vacuum_logical, detector_name + "_end_cap_vacuum", world_Logical, 0, 0, false);

  /************* Mount cup *************/
  
  // Mount cup side
  G4double mount_cup_inner_radius = properties.detector_radius + properties.dead_layer_side;
  G4double mount_cup_outer_radius = properties.detector_radius + properties.dead_layer_side + properties.mount_cup_thickness;
  G4double mount_cup_side_length = properties.mount_cup_length - properties.mount_cup_face_thickness - properties.mount_cup_base_thickness; // properties.mount_cup_length - properties.mount_cup_thickness - properties.mount_cup_base_thickness;
///*
  G4Tubs *mount_cup_side_solid = new G4Tubs(detector_name + "_mount_cup_side_solid", mount_cup_inner_radius, mount_cup_outer_radius, properties.mount_cup_length * 0.5, 0., twopi);
  G4LogicalVolume *mount_cup_side_logical = new G4LogicalVolume(mount_cup_side_solid, nist->FindOrBuildMaterial(properties.mount_cup_material), detector_name + "_mount_cup_side_logical");
  mount_cup_side_logical->SetVisAttributes(new G4VisAttributes(G4Color::Cyan()));
  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_length * 0.5), mount_cup_side_logical, detector_name + "_mount_cup_side", end_cap_vacuum_logical, 0, 0, false);
//*/ 
  G4double mount_cup_face_thickness = properties.mount_cup_face_thickness;
///*
  // Mount cup face
  if (mount_cup_face_thickness = properties.mount_cup_thickness) {
    G4Tubs *mount_cup_face_solid = new G4Tubs(detector_name + "_mount_cup_face_solid", 0., mount_cup_inner_radius, properties.mount_cup_face_thickness * 0.5, 0., twopi);
    G4LogicalVolume *mount_cup_face_logical = new G4LogicalVolume(mount_cup_face_solid, nist->FindOrBuildMaterial(properties.mount_cup_face_material), detector_name + "_mount_cup_face_logical");
    mount_cup_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Cyan()));
    new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_face_thickness * 0.5), mount_cup_face_logical, detector_name + "_mount_cup_face", end_cap_vacuum_logical, 0, 0, false);
  } 
  else if (mount_cup_face_thickness = properties.mount_cup_upper_window_thickness + properties.mount_cup_under_window_thickness) {
    // Mount cup upper window
    G4Tubs *mount_cup_upper_window_solid = new G4Tubs(detector_name + "_mount_cup_upper_window_solid", 0., mount_cup_inner_radius, properties.mount_cup_upper_window_thickness * 0.5, 0., twopi);
    G4LogicalVolume *mount_cup_upper_window_logical = new G4LogicalVolume(mount_cup_upper_window_solid, nist->FindOrBuildMaterial(properties.mount_cup_upper_window_material), detector_name + "_mount_cup_upper_window_logical");
    mount_cup_upper_window_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
    new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_upper_window_thickness * 0.5), mount_cup_upper_window_logical, detector_name + "_mount_cup_upper_window", end_cap_vacuum_logical, 0, 0, false);
    // Mount cup under window
    G4Tubs *mount_cup_under_window_solid = new G4Tubs(detector_name + "_mount_cup_under_window_solid", 0., mount_cup_inner_radius, properties.mount_cup_under_window_thickness * 0.5, 0., twopi);
    G4LogicalVolume *mount_cup_under_window_logical = new G4LogicalVolume(mount_cup_under_window_solid, nist->FindOrBuildMaterial(properties.mount_cup_under_window_material), detector_name + "_mount_cup_under_window_logical");
    mount_cup_under_window_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
    new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_upper_window_thickness + properties.mount_cup_under_window_thickness * 0.5), mount_cup_under_window_logical, detector_name + "_mount_cup_under_window", end_cap_vacuum_logical, 0, 0, false);
  }
  else if (mount_cup_face_thickness = properties.mount_cup_upper_window_thickness) {
    G4Tubs *mount_cup_face_solid = new G4Tubs(detector_name + "_mount_cup_face_solid", 0., mount_cup_inner_radius, properties.mount_cup_face_thickness * 0.5, 0., twopi);
    G4LogicalVolume *mount_cup_face_logical = new G4LogicalVolume(mount_cup_face_solid, nist->FindOrBuildMaterial(properties.mount_cup_face_material), detector_name + "_mount_cup_face_logical");
    mount_cup_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Cyan()));
    new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_face_thickness * 0.5), mount_cup_face_logical, detector_name + "_mount_cup_face", end_cap_vacuum_logical, 0, 0, false);
  }
  
  // Mount cup base
  G4Tubs *mount_cup_base_solid = new G4Tubs(detector_name + "_mount_cup_base_solid", properties.hole_radius, mount_cup_inner_radius, properties.mount_cup_base_thickness * 0.5, 0., twopi);
  G4LogicalVolume *mount_cup_base_logical = new G4LogicalVolume(mount_cup_base_solid, nist->FindOrBuildMaterial(properties.mount_cup_material), detector_name + "_mount_cup_base_logical");
  mount_cup_base_logical->SetVisAttributes(new G4VisAttributes(G4Color::Cyan()));
  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_face_thickness + mount_cup_side_length + 0.5 * properties.mount_cup_base_thickness), mount_cup_base_logical, detector_name + "_mount_cup_base", end_cap_vacuum_logical, 0, 0, false);
//*/
  /************* Cold finger *************/

 // G4double cold_finger_length = properties.cold_finger_penetration_depth + properties.mount_cup_face_thickness + mount_cup_side_length + properties.mount_cup_base_thickness - properties.hole_dead_layer - properties.detector_length - properties.dead_layer_top;

  const G4int nsteps = 500;

  G4double zPlaneTemp[nsteps];
  G4double rInnerTemp[nsteps];
  G4double rOuterTemp[nsteps];

  G4double z;
/*
  for (int i = 0; i < nsteps; i++) {
    z = (1. - (double)i / (nsteps - 1)) * cold_finger_length;

    zPlaneTemp[i] = z;

    rInnerTemp[i] = 0. * mm;

    if (z >= properties.cold_finger_radius) {
      rOuterTemp[i] = properties.cold_finger_radius;
    } else if (z >= 0.) {
      rOuterTemp[i] = properties.cold_finger_radius * sqrt(1. - pow((z - properties.cold_finger_radius) / properties.cold_finger_radius, 2));
    } else {
      rOuterTemp[i] = 0. * mm;
    }
  }
*/
  G4double zPlane[nsteps];
  G4double rInner[nsteps];
  G4double rOuter[nsteps];

  OptimizePolycone *opt = new OptimizePolycone();
/*  G4int nsteps_optimized = opt->Optimize(zPlaneTemp, rInnerTemp, rOuterTemp, zPlane, rInner, rOuter, nsteps, detector_name + "_cold_finger_solid");

  G4Polycone *cold_finger_solid = new G4Polycone(detector_name + "_cold_finger_solid", 0. * deg, 360. * deg, nsteps_optimized, zPlane, rInner, rOuter);

  G4LogicalVolume *cold_finger_logical = new G4LogicalVolume(cold_finger_solid, nist->FindOrBuildMaterial(properties.cold_finger_material), detector_name + "_cold_finger_logical", 0, 0, 0);

  cold_finger_logical->SetVisAttributes(new G4VisAttributes(G4Color(1.0, 0.5, 0.0)));

  new G4PVPlacement(0, G4ThreeVector(0., 0., end_cap_side_length * 0.5 - cold_finger_length), cold_finger_logical, detector_name + "_cold_finger", end_cap_vacuum_logical, 0, 0, false);
*/
  /************* Detector crystal *************/

  G4double heigth_of_curvature = properties.hole_face_radius - sqrt(pow(properties.hole_face_radius, 2) - pow(properties.hole_radius, 2)); // if hole_face_radius =/ hole_radius

  for (int i = 0; i < nsteps; i++) {
    z = (1. - (double)i / (nsteps - 1)) * properties.detector_length;

    zPlaneTemp[i] = z;
/*
    if (z>=properties.detector_length){
      rInnerTemp[i] = properties.hole_radius;
      rOuterTemp[i] = properties.detector_radius;
    } else {
      rInnerTemp[i] = 0.;
      rOuterTemp[i] = 0.;
    }
  }
*/
///*
    // rInnerTemp[i] = 0. * mm;
    if (z >= properties.detector_length - properties.hole_depth) {
      if (z >= properties.detector_length - properties.hole_depth + heigth_of_curvature) {
        rInnerTemp[i] = properties.hole_radius;
      } else {
        rInnerTemp[i] = properties.hole_radius * sqrt(1. - pow((z - (properties.detector_length - properties.hole_depth + heigth_of_curvature)) / properties.hole_radius, 2));
      }
    } else {
      rInnerTemp[i] = 0.;
    }

    if (z >= properties.detector_face_radius) {
      rOuterTemp[i] = properties.detector_radius;
    } else if (z >= 0.) {
      rOuterTemp[i] = properties.detector_face_radius * sqrt(1. - pow((z - properties.detector_face_radius) / properties.detector_face_radius, 2)) + (properties.detector_radius - properties.detector_face_radius);
    } else {
      rOuterTemp[i] = 0. * mm;
    }
  }
  //*/
  G4int nsteps_optimized = opt->Optimize(zPlaneTemp, rInnerTemp, rOuterTemp, zPlane, rInner, rOuter, nsteps, detector_name + "_crystal_solid");

  G4Polycone *crystal_solid = new G4Polycone("crystal_solid", 0. * deg, 360. * deg, nsteps_optimized, zPlane, rInner, rOuter);
  G4LogicalVolume *crystal_logical = new G4LogicalVolume(crystal_solid, nist->FindOrBuildMaterial("G4_Ge"), detector_name, 0, 0, 0);
  crystal_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));
  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_face_thickness + properties.dead_layer_top), crystal_logical, detector_name + "_crystal", end_cap_vacuum_logical, 0, 0, false);
  
  /******************** Inner dead layer *********************************/
///* 
  const G4int ksteps = 500;

  G4double zPlaneTempk[ksteps];
  G4double rInnerTempk[ksteps];
  G4double rOuterTempk[ksteps];

  for (int i = 0; i < ksteps; i++) {
    z = (1. - (double)i / (ksteps - 1)) * properties.hole_depth;

    zPlaneTempk[i] = z;

    if (z >= properties.hole_dead_layer) {
      if(z >= heigth_of_curvature - properties.hole_dead_layer){
        rInnerTempk[i] = properties.hole_radius - properties.hole_dead_layer;
      } else {
        rInnerTempk[i] = (properties.hole_radius - properties.hole_dead_layer) * sqrt(1. - pow((z - (heigth_of_curvature - properties.hole_dead_layer)) / (properties.hole_radius - properties.hole_dead_layer), 2));
      }    
    } else {
      rInnerTempk[i] = 0;
    }


    if (z >= heigth_of_curvature) {
      rOuterTempk[i] = properties.hole_radius;
    } else {
      rOuterTempk[i] = (properties.hole_radius) * sqrt(1. - pow((z - (heigth_of_curvature)) / (properties.hole_radius), 2));
    }
  }
  
  G4double zPlanek[ksteps];
  G4double rInnerk[ksteps];
  G4double rOuterk[ksteps];

  G4int ksteps_optimized = opt->Optimize(zPlaneTempk, rInnerTempk, rOuterTempk, zPlanek, rInnerk, rOuterk, ksteps, detector_name + "_hole_dead_layer_solid");

  G4Polycone *hole_dead_layer_solid = new G4Polycone("hole_dead_layer_solid", 0. * deg, 360. * deg, ksteps_optimized, zPlanek, rInnerk, rOuterk);
  G4LogicalVolume *hole_dead_layer_logical = new G4LogicalVolume(hole_dead_layer_solid, nist->FindOrBuildMaterial(properties.hole_dead_layer_material), detector_name + "_hole_dead_layer_logical", 0, 0, 0);
  hole_dead_layer_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_face_thickness + properties.dead_layer_top + properties.detector_length - properties.hole_depth), hole_dead_layer_logical, detector_name + "_hole_dead_layer", end_cap_vacuum_logical, 0, 0, false);
//*/  

  //if (properties.hole_dead_layer > 0) {
  //  G4Tubs *hole_dead_layer_tube_solid = new G4Tubs("hole_dead_layer_tube_solid", properties.hole_radius - properties.hole_dead_layer, properties.hole_radius, (properties.hole_depth - properties.hole_radius)/2, 0. *deg,  360. *deg);
  //  G4Sphere *hole_dead_layer_sphere_solid = new G4Sphere("hole_dead_layer_sphere_solid", properties.hole_radius - properties.hole_dead_layer, properties.hole_radius, 0. *deg, 360. *deg, 0. *deg, 90. *deg)
  //  G4ThreeVector move(0., 0., (properties.hole_depth) * 0.5)
  //  G4UnionSolid *hole_dead_layer_solid = new G4UnionSolid("hole_dead_layer_solid", hole_dead_layer_tube_solid, hole_dead_layer_sphere_solid, 0., move);
  //  G4LogicalVolume *hole_dead_layer_logical = new G4LogicalVolume(hole_dead_layer_solid, properties.hole_dead_layer_material, "hole_dead_layer_logical", 0, 0, 0);
  //  dead_layer_top_logical->SetVisAttributes(new G4VisAttributes(G4Color(1., 0.5, 1.)));
  //  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.dead_layer_top + properties.detector_length - properties.hole_depth * 0.5), hole_dead_layer_logical, detector_name + "_hole_dead_layer", end_cap_vacuum_logical, 0, 0, false); 
  //}
  
  /*********************** Outer dead layer ******************************/
///*  
  const G4int msteps = 500;

  G4double zPlaneTempm[msteps];
  G4double rInnerTempm[msteps];
  G4double rOuterTempm[msteps];


  for (int i = 0; i < msteps; i++) {
    z = (1. - (double)i / (msteps - 1)) * (properties.detector_length + properties.dead_layer_top);

    zPlaneTempm[i] = z;

    // rInnerTemp[i] = 0. * mm;
    if (z >= properties.dead_layer_top) {
      if (z >= properties.detector_face_radius + properties.dead_layer_top) {
        rInnerTempm[i] = properties.detector_radius;
      } else {
        rInnerTempm[i] = (properties.detector_face_radius) * sqrt(1. - pow((z - (properties.detector_face_radius)) / (properties.detector_face_radius), 2)) + ((properties.detector_radius) - (properties.detector_face_radius));
      }
    } else {
      rInnerTempm[i] = 0.;
    }

    if (z >= properties.detector_face_radius + properties.dead_layer_top) {
      rOuterTempm[i] = properties.detector_radius + 2 * properties.dead_layer_side;
    } else if (z >= 0.) {
      rOuterTempm[i] = (properties.detector_face_radius + 2 * properties.dead_layer_side) * sqrt(1. - pow((z - (properties.detector_face_radius + properties.dead_layer_top)) / (properties.detector_face_radius + 2 * properties.dead_layer_side), 2)) + ((properties.detector_radius + 2 * properties.dead_layer_side) - (properties.detector_face_radius + 2 * properties.dead_layer_side));
    } else {
      rOuterTempm[i] = 0. * mm;
    }
  }

  G4double zPlanem[msteps];
  G4double rInnerm[msteps];
  G4double rOuterm[msteps];

  G4int msteps_optimized = opt->Optimize(zPlaneTempm, rInnerTempm, rOuterTempm, zPlanem, rInnerm, rOuterm, msteps, detector_name + "_outer_dead_layer_solid");

  G4Polycone *outer_dead_layer_solid = new G4Polycone("outer_dead_layer_solid", 0. * deg, 360. * deg, msteps_optimized, zPlanem, rInnerm, rOuterm);
  G4LogicalVolume *outer_dead_layer_logical = new G4LogicalVolume(outer_dead_layer_solid, nist->FindOrBuildMaterial(properties.dead_layer_material), detector_name + "_outer_dead_layer_logical", 0, 0, 0);
  outer_dead_layer_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.mount_cup_face_thickness), outer_dead_layer_logical, detector_name + "_outer_dead_layer", end_cap_vacuum_logical, 0, 0, false);
//*/  
  //if (properties.dead_layer_top > 0) {
  //  G4Tubs *dead_layer_top_solid = new G4Tubs("dead_layer_top_solid", 0., properties.detector_radius + properties.dead_layer_side, properties.dead_layer_top/2, 0. *deg,  360. *deg);
  //  G4LogicalVolume *dead_layer_top_logical = new G4LogicalVolume(dead_layer_top_solid, properties.dead_layer_material, detector_name + "_dead_layer_top_logical", 0, 0, 0);
  //  dead_layer_top_logical->SetVisAttributes(new G4VisAttributes(G4Color(1., 0.5, 1.)));
  //  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.dead_layer_top * 0.5), dead_layer_top_logical, detector_name + "_dead_layer_top", end_cap_vacuum_logical, 0, 0, false); 
  //}
  //if (properties.dead_layer_side > 0) {
  //  G4Tubs *dead_layer_side_solid = new G4Tubs("dead_layer_side_solid", properties.detector_radius, properties.detector_radius +  properties.dead_layer_side, properties.detector_length/2, 0. *deg, 360. *deg);
  //  G4LogicalVolume *dead_layer_side_logical = new G4LogicalVolume(dead_layer_side_solid, properties.dead_layer_material, detector_name + "_dead_layer_side_logical", 0, 0, 0);
  //  dead_layer_side_logical->SetVisAttributes(new G4VisAttributes(G4Color(1., 0.5, 1.)));
  //  new G4PVPlacement(0, G4ThreeVector(0., 0., -end_cap_side_length * 0.5 + properties.end_cap_to_crystal_gap_front + properties.dead_layer_top + properties.detector_length) * 0.5, dead_layer_side_logical, detector_name + "_dead_layer_side", end_cap_vacuum_logical, 0, 0, false); 
  //}

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////// From here on, the code was not adapted to the new variables!! /////////////////////////////////////////////

  if (use_dewar) {
    /************* Connection dewar-detector *************/
    G4Tubs *connection_solid = new G4Tubs(detector_name + "_dewar_connection_solid", 0., properties.connection_radius, properties.connection_length * 0.5, 0., twopi);
    G4LogicalVolume *connection_logical = new G4LogicalVolume(connection_solid, nist->FindOrBuildMaterial(properties.connection_material), detector_name + "_dewar_connection_logical");
    connection_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
    new G4PVPlacement(rotation, global_coordinates + (dist_from_center + properties.end_cap_window_thickness + end_cap_side_length + properties.connection_length * 0.5) * symmetry_axis, connection_logical, detector_name + "_dewar_connection", world_Logical, 0, 0, false);

    if (intrinsic_rotation_angle != 0.)
      symmetry_axis_orthogonal.rotate(intrinsic_rotation_angle, symmetry_axis);

    /************* Dewar *************/
    // Dewar face
    G4Tubs *dewar_solid = new G4Tubs(detector_name + "_dewar_solid", 0., properties.dewar_outer_radius, properties.dewar_length * 0.5, 0., twopi);
    G4LogicalVolume *dewar_logical = new G4LogicalVolume(dewar_solid, nist->FindOrBuildMaterial(properties.dewar_material), detector_name + "_dewar_logical");
    dewar_logical->SetVisAttributes(G4Color::Brown());
    new G4PVPlacement(rotation, global_coordinates + (dist_from_center + properties.end_cap_window_thickness + end_cap_side_length + properties.connection_length + properties.dewar_length * 0.5) * symmetry_axis, dewar_logical, detector_name + "_dewar", world_Logical, 0, 0, false);

    // Dewar interior
    G4Tubs *dewar_interior_solid = new G4Tubs(detector_name + "_dewar_interior_solid", 0., properties.dewar_outer_radius - properties.dewar_wall_thickness, properties.dewar_length * 0.5 - properties.dewar_wall_thickness, 0., twopi);
    G4LogicalVolume *dewar_interior_logical = new G4LogicalVolume(dewar_interior_solid, nist->FindOrBuildMaterial("G4_N"), detector_name + "_dewar_interior_logical");
    dewar_interior_logical->SetVisAttributes(G4Color::Red());
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), dewar_interior_logical, detector_name + "_dewar_interior", dewar_logical, 0, 0, false);
  }

  // Filters
  G4double filter_position_z = 0.; // Will be gradually increased to be able to place filters on top of each other

  // Filter case
  Filter_Case filter_case(world_Logical, detector_name);
  if (use_filter_case_ring) {
    filter_case.Construct_Ring(global_coordinates, theta, phi, dist_from_center - filter_case.get_filter_case_ring_thickness() / 2.);
    filter_position_z = filter_position_z + filter_case.get_filter_case_ring_thickness();
  }

  if (filter_materials.size()) {
    G4Tubs *filter_solid = nullptr;
    G4LogicalVolume *filter_logical = nullptr;
    G4double filter_radius;
    stringstream filter_base_name_ss;
    for (unsigned int i = 0; i < filter_materials.size(); ++i) {
      filter_radius = (filter_radii[i] < 0.) ? end_cap_outer_radius : filter_radii[i]; // A negative filter radius value tells us to use the detector front radius as the filter radius
      filter_base_name_ss << detector_name << "_filter_" << i + 1 << "_" << filter_materials[i] << "_" << filter_thicknesses[i] / mm << "mm_x_" << filter_radius / mm << "mm";
      filter_solid = new G4Tubs(filter_base_name_ss.str() + "_solid", 0., filter_radius, filter_thicknesses[i] / 2., 0., twopi);
      filter_logical = new G4LogicalVolume(filter_solid, nist->FindOrBuildMaterial(filter_materials[i]), filter_base_name_ss.str() + "_logical");
      if (i % 2 == 0) {
        filter_logical->SetVisAttributes(G4Color::Red());
      } else {
        filter_logical->SetVisAttributes(G4Color::Green());
      }
      new G4PVPlacement(rotation, global_coordinates + (dist_from_center - filter_position_z - filter_thicknesses[i] / 2.) * symmetry_axis, filter_logical, filter_base_name_ss.str(), world_Logical, 0, 0, false);
      filter_position_z = filter_position_z + filter_thicknesses[i];
      filter_base_name_ss.str("");
    }
  }

  if (use_filter_case) {
    filter_case.Construct_Case(global_coordinates, theta, phi, dist_from_center - filter_case.get_filter_case_bottom_thickness() / 2. - filter_position_z);
  }

  // Wraps
  if (wrap_materials.size()) {
    G4Tubs *wrap_solid = nullptr;
    G4LogicalVolume *wrap_logical = nullptr;
    G4double wrap_radius = end_cap_outer_radius; // Will be gradually increased to be able to place wraps on top of each other
    stringstream wrap_base_name_ss;
    for (unsigned int i = 0; i < wrap_materials.size(); ++i) {
      wrap_base_name_ss << detector_name << "_wrap_" << i + 1 << "_" << wrap_materials[i] << "_" << wrap_thicknesses[i] / mm << "mm";
      wrap_solid = new G4Tubs(wrap_base_name_ss.str() + "_solid", wrap_radius, wrap_radius + wrap_thicknesses[i], properties.end_cap_length / 2., 0., twopi);
      wrap_logical = new G4LogicalVolume(wrap_solid, nist->FindOrBuildMaterial(wrap_materials[i]), wrap_base_name_ss.str() + "_logical");
      if (i % 2 == 0) {
        wrap_logical->SetVisAttributes(G4Color::Green());
      } else {
        wrap_logical->SetVisAttributes(G4Color::Red());
      }
      new G4PVPlacement(rotation, global_coordinates + (dist_from_center + properties.end_cap_length / 2.) * symmetry_axis, wrap_logical, wrap_base_name_ss.str(), world_Logical, 0, 0, false);
      wrap_radius = wrap_radius + wrap_thicknesses[i];
      wrap_base_name_ss.str("");
    }
  }
}

void HPGe_Coaxial::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                             G4double dist_from_center) const {
  Construct(global_coordinates, theta, phi, dist_from_center, 0.);
}
