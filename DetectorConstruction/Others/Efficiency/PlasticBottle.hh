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

class Plastic_Bottle {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *PlasticBottle_Logical;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Plastic_Bottle(G4LogicalVolume *world_Logical) {
    // dimensions of marinelli
    // Vorsicht: Deckel ragt ca. 8 mm in den Becher rein

    // OuterCone


    // color of marinelli
    G4Colour magenta(1.0, 0.0, 1.0);

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    World_Logical = world_Logical;

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    //if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
    //  gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_aussen.gdml";
    //}
    //else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
    //  gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_aussen.gdml";
    //} 

    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_aussen_Wandung_1_15.gdml";
    }
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_aussen_Wandung_1_15.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    PlasticBottle_Logical = gdmlWorld->GetLogicalVolume();

    //Rundflasche_Solid = new G4Polycone("Rundflasche_Solid", 0, 360.*deg, 3, z1, ri, r1);
    
    //Rundflasche_Logical = new G4LogicalVolume(Rundflasche_Solid, polyethylene, "Rundflasche_Logical", 0, 0, 0);
    
    PlasticBottle_Logical->SetVisAttributes(new G4VisAttributes(magenta));
    PlasticBottle_Logical->SetMaterial(polyethylene);
   
    rot = new G4RotationMatrix();
    
  }

  ~Plastic_Bottle(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), PlasticBottle_Logical,
                      "Plastic_Bottle", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), PlasticBottle_Logical,
                      "Plastic_Bottle", World_Logical, false, 0);
  }
};

class Plastic_Bottle_Filled {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *PlasticBottleFilled_Logical;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Plastic_Bottle_Filled(G4LogicalVolume *world_Logical) {
    // dimensions of marinelli
    // Vorsicht: Deckel ragt ca. 8 mm in den Becher rein
    // color of marinelli
    G4Colour red(1.0, 0.0, 0.0);
    
    World_Logical = world_Logical;
    
    G4NistManager* nist = G4NistManager::Instance();  

    /********************************* Füllmedien der Flasche ******************************************/

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    //G4NistManager *nist = G4NistManager::Instance();
    //G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    //G4double density = {{density}} * g/cm3; // 0.839 * g/cm3;
    //G4double density_alo = 0.768 *g/cm3; //(1680.01 / 2198) * g/cm3; // 0.76 * g/cm3;

    // Wandung 1.25 mm
    //G4double density_alo = ((1861-180.99) / 2198) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 244 mm
    //G4double density_alo = ((1861-180.99) / 2126) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 199.4 mm
    //G4double density_alo = ((1861-180.99) / 2121) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 198.9 mm
    //G4double density_alo = ((1861-180.99) / 2115) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 198.4 mm

    // Wandung 1.15 mm
    //G4double density_alo = ((1861-180.99) / 2208) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 244 mm
    //G4double density_alo = ((1861-180.99) / 2135) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 199.4 mm
    //G4double density_alo = ((1861-180.99) / 2130) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 198.9 mm
    G4double density_alo = ((1861-180.99) / 2125) * g/cm3; // 0.76 * g/cm3; fuer Füllhöhe 198.4 mm

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
    //if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
    //  gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_innen.gdml";
    //}
    //else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
    //  gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_innen.gdml";
    //} 

    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_innen_Wandung_1_15.gdml";
    }
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/PE_Flasche_innen_Wandung_1_15.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    PlasticBottleFilled_Logical = gdmlWorld->GetLogicalVolume();

    PlasticBottleFilled_Logical->SetVisAttributes(new G4VisAttributes(red));
    PlasticBottleFilled_Logical->SetMaterial(AlO);
   
    rot = new G4RotationMatrix();
    
  }

  ~Plastic_Bottle_Filled(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), PlasticBottleFilled_Logical,
                      "Plastic_Bottle_Filled", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), PlasticBottleFilled_Logical,
                      "Plastic_Bottle_Filled", World_Logical, false, 0);
  }
};

class Bottle_Holder {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *BottleHolder_Logical;
  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Bottle_Holder(G4LogicalVolume *world_Logical) {
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
      gdmlFileName = "DetectorConstruction/Others/Efficiency/Volumes/Flaschenhalter.gdml";
    }
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/Flaschenhalter.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    BottleHolder_Logical = gdmlWorld->GetLogicalVolume();

    //Rundflasche_Solid = new G4Polycone("Rundflasche_Solid", 0, 360.*deg, 3, z1, ri, r1);
    
    //Rundflasche_Logical = new G4LogicalVolume(Rundflasche_Solid, polyethylene, "Rundflasche_Logical", 0, 0, 0);
    
    BottleHolder_Logical->SetVisAttributes(new G4VisAttributes(green));
    BottleHolder_Logical->SetMaterial(polyethylene);
   
    rot = new G4RotationMatrix();
    
  }

  ~Bottle_Holder(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), BottleHolder_Logical,
                      "Bottle_Holder", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), BottleHolder_Logical,
                      "Bottle_Holder", World_Logical, false, 0);
  }
};