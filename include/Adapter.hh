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

//#include "DetectorConstruction.hh"

class Adapter {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *Adapter_Logical;
  G4Tubs *Adapter_Solid;
  G4RotationMatrix *rot;

  public:
  G4double zylinder_lenght;
  G4double zylinder_inner_radius;
  G4double zylinder_outer_radius;
  G4double zylinder_sphi;
  G4double zylinder_dphi;

  G4double hohlzylinder_lenght;
  G4double hohlzylinder_inner_radius;
  G4double hohlzylinder_outer_radius;
  G4double hohlzylinder_sphi;
  G4double hohlzylinder_dphi;

  Adapter(G4LogicalVolume *world_Logical) {
    zylinder_lenght = 0.5 * 30. * mm; // Measured
    zylinder_inner_radius = 0. * mm;
    zylinder_outer_radius = 45.5 * mm; // Measured
    zylinder_sphi = 0. * deg;
    zylinder_dphi = 180. * deg;

    hohlzylinder_lenght = 0.5 * 20. * mm; // Measured
    hohlzylinder_inner_radius = 40.5 * mm; // Measured
    hohlzylinder_outer_radius = 45.5 * mm; // Measured
    hohlzylinder_sphi = 0. * deg;
    hohlzylinder_dphi = 180. * deg;



    G4Colour yellow(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Plastic = nist->FindOrBuildMaterial("G4_PLASTIC");

    World_Logical = world_Logical;

    Adapter_Solid = new G4Tubs("Adapter_Solid", zylinder_inner_radius, zylinder_outer_radius, zylinder_lenght, zylinder_sphi, zylinder_dphi);
    Adapter_Logical = new G4LogicalVolume(Adapter_Solid, Plastic, "Adapter_Logical", 0, 0, 0);

    Adapter_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    rot = new G4RotationMatrix();
  }

  ~Adapter(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), Adapter_Logical,
                      "Adapter", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), Adapter_Logical,
                      "Adapter", World_Logical, false, 0);
  }
};