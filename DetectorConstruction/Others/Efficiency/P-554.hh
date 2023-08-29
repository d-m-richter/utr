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
#pragma once

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// Define ProbenhalerLang for Eurysis Mesures HPGe detector in the isotop laboratory.
// Is placed on top of the adapter which is placed on top of the detector end cap.
// The ProbenhalterLang was printed with a 3D printer.
// The exact material has yet to be found out. Assumption: PLA (polylactic acid)) 

class P554 {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *P554_Logical;
  G4Tubs *P554_Solid;
  G4RotationMatrix *rot;

  public:
  G4double lenght;
  G4double radius;
  G4double sphi;
  G4double dphi;

  P554(G4LogicalVolume *world_Logical) {
    // dimensions of ProbenhalterLang
    lenght = 0.5 * 19.15 * mm; // Measured
    radius = 0.5 * 17.85 * mm; // Measured
    sphi = 0. * deg;
    dphi = 360. * deg;

    // color of ProbenhalterLang
    G4Colour grey(0.5, 0.5, 0.5);

    // define material PLA
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *brass = nist->FindOrBuildMaterial("G4_BRASS");

    World_Logical = world_Logical;

    P554_Solid = new G4Tubs("P554_Solid", 0., radius, lenght, sphi, dphi);

    P554_Logical = new G4LogicalVolume(P554_Solid, brass, "P554_Logical", 0, 0, 0);
    
    P554_Logical->SetVisAttributes(new G4VisAttributes(grey));
   
    rot = new G4RotationMatrix();
  }

  ~P554(){};

  // placing the ProbenhalterLang in the detector world with Put() methode
  // use ProbenhalterLang *probenhalterLang = new ProbenhalterLang(world_logical) and probenhalterLang->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), P554_Logical,
                      "P554", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), P554_Logical,
                      "P554", World_Logical, false, 0);
  }
};


