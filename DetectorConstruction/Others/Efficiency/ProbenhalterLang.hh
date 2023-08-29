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

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "Units.hh" 

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4GDMLParser.hh"
#include "G4TessellatedSolid.hh"

// Define ProbenhalerLang for Eurysis Mesures HPGe detector in the isotop laboratory.
// Is placed on top of the adapter which is placed on top of the detector end cap.
// The ProbenhalterLang was printed with a 3D printer.
// The exact material has yet to be found out. Assumption: PLA (polylactic acid)) 

class ProbenhalterLang {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *ProbenhalterLang_Logical;
  //G4Tubs *Zylinder_Solid;
  //G4Tubs *Hohlzylinder_Solid;
  //G4UnionSolid *ProbenhalterLang_Solid;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  //G4double zylinder_lenght;
  //G4double zylinder_inner_radius;
  //G4double zylinder_outer_radius;
  //G4double sphi;
  //G4double dphi;
  //G4double hohlzylinder_lenght;
  //G4double hohlzylinder_inner_radius;
  //G4double hohlzylinder_outer_radius;
  G4String gdmlFileName;

  ProbenhalterLang(G4LogicalVolume *world_Logical) {
    // dimensions of ProbenhalterLang
    //zylinder_lenght = 0.5 * 60. * mm; // Measured
    //zylinder_inner_radius = 0. * mm;
    //zylinder_outer_radius = 15. * mm; // Measured
    //sphi = 0. * deg;
    //dphi = 360. * deg;
    //hohlzylinder_lenght = 0.5 * 4. * mm; // Measured
    //hohlzylinder_inner_radius = 9.5 * mm; // Measured
    //hohlzylinder_outer_radius = 15. * mm; // Measured

    // color of ProbenhalterLang
    G4Colour grey(0.5, 0.5, 0.5);

    // define material PLA
    //G4double a, z, density; // a = mass of a mole, z = mean number of protons;
    //G4Element* elH = new G4Element("Hydrogen","H", z=1., a=1.01*g/mole);
    //G4Element* elC = new G4Element("Carbon","C", z=6, a=12.01*g/mole);
    //G4Element* elO = new G4Element("Oxygen","O", z=8, a=16.00*g/mole);
    //G4int ncomponents, natoms;
    //G4Material* PLA = new G4Material("Polylatic", density=1.320*g/cm3, ncomponents=3);
    //PLA->AddElement(elC, natoms=3);
    //PLA->AddElement(elH, natoms=4);
    //PLA->AddElement(elO, natoms=2);

    World_Logical = world_Logical;

    //Zylinder_Solid = new G4Tubs("Zylinder_Solid", zylinder_inner_radius, zylinder_outer_radius, zylinder_lenght, sphi, dphi);
    //Hohlzylinder_Solid = new G4Tubs("Hohlzylinder_Solid", hohlzylinder_inner_radius, hohlzylinder_outer_radius, hohlzylinder_lenght, sphi, dphi);
    
    gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/ProbenhalterLang.gdml";
    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    ProbenhalterLang_Logical = gdmlWorld->GetLogicalVolume();

    // joining the two cylinders to form the ProbenhalterLang
    //G4ThreeVector move(0.,0.,32.);
    //ProbenhalterLang_Solid = new G4UnionSolid("ProbenhalterLang_Solid", Hohlzylinder_Solid, Zylinder_Solid, 0, move);

    //ProbenhalterLang_Logical = new G4LogicalVolume(ProbenhalterLang_Solid, PLA, "ProbenhalterLang_Logical", 0, 0, 0);
    
    ProbenhalterLang_Logical->SetVisAttributes(new G4VisAttributes(grey));
   
    rot = new G4RotationMatrix();
  }

  ~ProbenhalterLang(){};

  // placing the ProbenhalterLang in the detector world with Put() methode
  // use ProbenhalterLang *probenhalterLang = new ProbenhalterLang(world_logical) and probenhalterLang->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), ProbenhalterLang_Logical,
                      "ProbenhalterLang", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), ProbenhalterLang_Logical,
                      "ProbenhalterLang", World_Logical, false, 0);
  }
};