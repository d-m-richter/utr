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
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4GDMLParser.hh"
#include "G4TessellatedSolid.hh"

// Define ProbenhalerKurz for Eurysis Mesures HPGe detector in the isotop laboratory.
// Is placed on top of the adapter which is placed on top of the detector end cap.
// The ProbenhalterKurz was printed with a 3D printer.
// The exact material has yet to be found out. Assumption: PLA (polylactic acid)) 

class Quellenhalter {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *Quellenhalter_Logical;

  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Quellenhalter(G4LogicalVolume *world_Logical) {

    // color of Quellenhalter
    G4Colour grey(0.5, 0.5, 0.5);

    World_Logical = world_Logical;

    gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/Quellenhalter.gdml";
    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    Quellenhalter_Logical = gdmlWorld->GetLogicalVolume();
    
    Quellenhalter_Logical->SetVisAttributes(new G4VisAttributes(grey));
   
    rot = new G4RotationMatrix();
  }

  ~Quellenhalter(){};

  // placing the ProbenhalterKurz in the detector world with Put() methode
  // use ProbenhalterKurz *probenhalterKurz = new ProbenhalterKurz(world_logical) and probenhalterKurz->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), Quellenhalter_Logical,
                      "Quellenhalter", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), Quellenhalter_Logical,
                      "Quellenhalter", World_Logical, false, 0);
  }
};