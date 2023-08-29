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

class P660 {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *P660_Logical;
  G4Tubs *oben_Solid;
  G4Tubs *mitte_Solid;
  G4Tubs *unten_Solid;
  G4UnionSolid *obenunten_Solid;
  G4UnionSolid *P660_Solid;
  G4RotationMatrix *rot;

  public:
  G4double oben_lenght;
  G4double oben_radius;
  G4double mitte_lenght;
  G4double mitte_radius;
  G4double unten_lenght;
  G4double unten_radius;
  G4double sphi;
  G4double dphi;

  P660(G4LogicalVolume *world_Logical) {
    // dimensions of ProbenhalterLang
    oben_lenght = 0.5 * 6.59 * mm; // Measured
    oben_radius = 0.5 * 17.8 * mm; // Measured
    mitte_lenght = 0.5 * 14.1329 * mm; // Measured
    mitte_radius = 0.5 * 16. * mm; // Measured
    unten_lenght = 0.5 * 1.9771 * mm; // Measured
    unten_radius = 0.5 * 17.8 * mm; // Measured
    sphi = 0. * deg;
    dphi = 360. * deg;

    // color of ProbenhalterLang
    G4Colour grey(0.5, 0.5, 0.5);

    // define material PLA
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *brass = nist->FindOrBuildMaterial("G4_BRASS");

    World_Logical = world_Logical;

    oben_Solid = new G4Tubs("Oben_Solid", 0., oben_radius, oben_lenght, sphi, dphi);
    mitte_Solid = new G4Tubs("Mitte_Solid", 0., mitte_radius, mitte_lenght, sphi, dphi);
    unten_Solid = new G4Tubs("Unten_Solid", 0., unten_radius, unten_lenght, sphi, dphi);
    
    // joining the two cylinders to form the ProbenhalterLang
    G4ThreeVector m1(0.,0.,10.36145);
    obenmitte_Solid = new G4UnionSolid("ObenMitte_Solid", oben_Solid, mitte_Solid, 0, m1);
    G4ThreeVector m2(0.,0.,11.35);
    P660_Solid = new G4UnionSolid("P660_Solid", obenmitte_Solid, unten_Solid, 0, m2);

    P660_Logical = new G4LogicalVolume(P660_Solid, brass, "P660_Logical", 0, 0, 0);
    
    P660_Logical->SetVisAttributes(new G4VisAttributes(grey));
   
    rot = new G4RotationMatrix();
  }

  ~P660(){};

  // placing the ProbenhalterLang in the detector world with Put() methode
  // use ProbenhalterLang *probenhalterLang = new ProbenhalterLang(world_logical) and probenhalterLang->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), P660_Logical,
                      "P660", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), P660_Logical,
                      "P660", World_Logical, false, 0);
  }
};
