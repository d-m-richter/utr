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

#include <iostream>
#include <fstream>
#include <filesystem>

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

// Define adaptor for Eurysis Mesures HPGe detector in the isotop laboratory.
// Is placed on top of the detector end cap.
// The adapter was printed with a 3D printer.
// The exact material has yet to be found out. Assumption: PLA (polylactic acid)) 

class Marinelli_Beaker {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *MarinelliBeaker_Logical;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Marinelli_Beaker(G4LogicalVolume *world_Logical) {
    // dimensions of marinelli
    // Vorsicht: Deckel ragt ca. 8 mm in den Becher rein

    // OuterCone


    // color of marinelli
    G4Colour magenta(1.0, 0.0, 1.0);

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polypropylene = nist->FindOrBuildMaterial("G4_POLYPROPYLENE");

    World_Logical = world_Logical;

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/MarinelliBecher_aussen.gdml";
    }
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/MarinelliBecher_aussen.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    MarinelliBeaker_Logical = gdmlWorld->GetLogicalVolume();

    MarinelliBeaker_Logical->SetVisAttributes(new G4VisAttributes(magenta));
    MarinelliBeaker_Logical->SetMaterial(polypropylene);
   
    rot = new G4RotationMatrix();
    
  }

  ~Marinelli_Beaker(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), MarinelliBeaker_Logical,
                      "Marinelli_Beaker", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), MarinelliBeaker_Logical,
                      "Marinelli_Beaker", World_Logical, false, 0);
  }
};

class Marinelli_Beaker_Filled {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *MarinelliBeakerFilled_Logical;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Marinelli_Beaker_Filled(G4LogicalVolume *world_Logical) {
    // dimensions of marinelli
    // Vorsicht: Deckel ragt ca. 8 mm in den Becher rein
    // color of marinelli
    G4Colour red(1.0, 0.0, 0.0);
    
    World_Logical = world_Logical;
    
    G4NistManager* nist = G4NistManager::Instance();  

    /********************************* Füllmedien der Marinelli Becher ******************************************/

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    //G4NistManager *nist = G4NistManager::Instance();
    //G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    //G4double density = {{density}} * g/cm3; // 0.839 * g/cm3;
    //G4double density_alo = ((2897.63 - 351.94) / 4100) * g/cm3; // 0.76 * g/cm3;
    //G4double density_alo = ((2897.63 - 351.94) / 3186) * g/cm3; // 0.76 * g/cm3;
    //G4double density_alo = ((2897.63 - 351.94) / 3339) * g/cm3; // 0.76 * g/cm3;
    //G4double density_alo = ((2897.63 - 351.94) / 3491) * g/cm3; // 0.76 * g/cm3;
    //G4double density_alo = ((2897.63 - 351.94) / 3813) * g/cm3; // 0.76 * g/cm3;
    //G4double density_alo = ((2897.63 - 351.94) / 3956) * g/cm3; // 0.76 * g/cm3;
    G4double density_alo = ((2897.63 - 351.94) / 3670) * g/cm3; // 0.76 * g/cm3;
    G4Material* AlO = nist->BuildMaterialWithNewDensity("G4_ALUMINIUM_OXIDE_new","G4_ALUMINUM_OXIDE",density_alo);

    // Petrischale gefüllt mit Eisenpfeilspähnen
    G4double density_fe = 2.8766 * g/cm3; // Schüttdichte
    //G4Material* Fe = nist->FindOrBuildMaterial("G4_Fe");
    G4Material* Fe = nist->BuildMaterialWithNewDensity("G4_Fe_new","G4_Fe",density_fe);

    // Aluminium
    G4Material* Al = nist->FindOrBuildMaterial("G4_Al");

    // Wasser
    G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");

    // Handschuhe aus Nitrilkautschuk
    // CAS-Nr.: 9003-18-3 Poly(acrylonitril-co-butadien) 
    G4double density_nitril = 0.3065 * g/cm3; // Schüttdichte
    G4Material* Nitril = new G4Material("Nitril", density_nitril, 3);
    Nitril->AddElement(nist->FindOrBuildElement("C"), 7);
    Nitril->AddElement(nist->FindOrBuildElement("H"), 9);
    Nitril->AddElement(nist->FindOrBuildElement("N"), 1);

    /*********************************************************************************************************/

    

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/MarinelliBecher_innen.gdml";
    }
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/MarinelliBecher_innen.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    MarinelliBeakerFilled_Logical = gdmlWorld->GetLogicalVolume();

    MarinelliBeakerFilled_Logical->SetVisAttributes(new G4VisAttributes(red));
    MarinelliBeakerFilled_Logical->SetMaterial(Nitril);
   
    rot = new G4RotationMatrix();
    
  }

  ~Marinelli_Beaker_Filled(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), MarinelliBeakerFilled_Logical,
                      "Marinelli_Beaker_Filled", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), MarinelliBeakerFilled_Logical,
                      "Marinelli_Beaker_Filled", World_Logical, false, 0);
  }
};

class Becher_Halterung {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *BecherHalterung_Logical;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Becher_Halterung(G4LogicalVolume *world_Logical) {
    // dimensions of marinelli
    // Vorsicht: Deckel ragt ca. 8 mm in den Becher rein

    // OuterCone


    // color of marinelli
    G4Colour green(0.0, 1.0, 0.0);

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    World_Logical = world_Logical;

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/BecherHalterung.gdml";
    }
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/BecherHalterung.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    BecherHalterung_Logical = gdmlWorld->GetLogicalVolume();

    //Rundflasche_Solid = new G4Polycone("Rundflasche_Solid", 0, 360.*deg, 3, z1, ri, r1);
    
    //Rundflasche_Logical = new G4LogicalVolume(Rundflasche_Solid, polyethylene, "Rundflasche_Logical", 0, 0, 0);
    
    BecherHalterung_Logical->SetVisAttributes(new G4VisAttributes(green));
    BecherHalterung_Logical->SetMaterial(polyethylene);
   
    rot = new G4RotationMatrix();
    
  }

  ~Becher_Halterung(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), BecherHalterung_Logical,
                      "Becher_Halterung", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), BecherHalterung_Logical,
                      "Becher_Halterung", World_Logical, false, 0);
  }
};