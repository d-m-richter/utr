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

#include "G4PhysicalConstants.hh"

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

#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"

// Define ProbenhalerKurz for Eurysis Mesures HPGe detector in the isotop laboratory.
// Is placed on top of the adapter which is placed on top of the detector end cap.
// The ProbenhalterKurz was printed with a 3D printer.
// The exact material has yet to be found out. Assumption: PLA (polylactic acid)) 

class Petrischale {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *Petrischale_Logical;

  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Petrischale(G4LogicalVolume *world_Logical) {

    // The petri dish that is read in here is without thread (Gewinde), because the gdml file has to many points to be simulated and GEANT4 has a problem with that.

    // color of Hohlzylinder
    G4Colour grey(0.5, 0.5, 0.5);

    World_Logical = world_Logical;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    ///*  
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    //if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection") {
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "../utr/DetectorConstruction/Others/Efficiency/Volumes/Petrischale.gdml";
    }
    //else if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection/build") {
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/Petrischale.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();
    
    Petrischale_Logical = gdmlWorld->GetLogicalVolume();
    
    Petrischale_Logical->SetVisAttributes(new G4VisAttributes(grey));
    Petrischale_Logical->SetMaterial(polyethylene);
    //*/
    /*

    G4Tubs *tub1 = new G4Tubs("Tub1", 0 *mm, 50. *mm, 0.6/2. *mm, 0 *deg, 360 *deg);
    G4Tubs *tub2 = new G4Tubs("Tub2", 45. *mm, 50. *mm, 8./2. *mm, 0 *deg, 360 *deg);
    G4Tubs *tub3 = new G4Tubs("Tub3", 40. *mm, 45. *mm, 14.15/2. *mm, 0 *deg, 360 *deg);
    G4Tubs *tub4 = new G4Tubs("Tub4", 0 *mm, 45. *mm, 0.6/2. *mm, 0 *deg, 360 *deg);

    G4UnionSolid *union1 = new G4UnionSolid("Union1", tub1, tub2, 0, G4ThreeVector(0, 0, -(0.6/2. + 8./2.)));
    G4UnionSolid *union2 = new G4UnionSolid("Union2", union1, tub3, 0, G4ThreeVector(0, 0, -(0.6 + 14.15/2.)));
    G4UnionSolid *union3 = new G4UnionSolid("Union3", union2, tub4, 0, G4ThreeVector(0, 0, -(0.6/2. + 14.75)));

    Petrischale_Logical = new G4LogicalVolume(union3, polyethylene, "Petrischale_Logical");
    Petrischale_Logical->SetVisAttributes(new G4VisAttributes(grey));
    */
    rot = new G4RotationMatrix();
  }

  ~Petrischale(){};

  // placing the ProbenhalterKurz in the detector world with Put() methode
  // use ProbenhalterKurz *probenhalterKurz = new ProbenhalterKurz(world_logical) and probenhalterKurz->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), Petrischale_Logical,
                      "Petrischale", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), Petrischale_Logical,
                      "Petrischale", World_Logical, false, 0);
  }
};

class Fuelvolumen {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *Fuelvolumen_Logical;

  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Fuelvolumen(G4LogicalVolume *world_Logical) {

    // color of Hohlzylinder_GEM
    G4Colour magenta(1., 0., 1.);

    World_Logical = world_Logical;

    G4NistManager *nist = G4NistManager::Instance();

    /********************************* Füllmedien der Petrischale ******************************************/
    
    // Petrischale gefüllt mit Aluminiumoxid (Granulat)
    G4double density_alo = (34.2 / 71125.658) * g/mm3; // Schüttdichte
    //G4Material* AlO = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
    G4Material* AlO = nist->BuildMaterialWithNewDensity("G4_ALUMINIUM_OXIDE_new","G4_ALUMINUM_OXIDE",density_alo);

    // Petrischale gefüllt mit Eisenpfeilspähnen
    G4double density_fe = (204.6 / 71125.658) * g/mm3; // Schüttdichte
    //G4Material* Fe = nist->FindOrBuildMaterial("G4_Fe");
    G4Material* Fe = nist->BuildMaterialWithNewDensity("G4_Fe_new","G4_Fe",density_fe);

    // Kappasorb ist nicht in der NIST-Datenbank enthalten, weshalb es hier extra erstellt werden muss.
    // Daten vom Sicherheitsdatenblatt: CAS-Nr.: 9003-04-07 Natriumpolyacrylat, quervernetzt, Dichte: 0.7 g/cm3
    G4double density_kappa = 0.7 * g/cm3; // Dichte
    G4Material* Kappasorb = new G4Material("Kappasorb", density_kappa, 4);
    Kappasorb->AddElement(nist->FindOrBuildElement("C"), 3);
    Kappasorb->AddElement(nist->FindOrBuildElement("H"), 3);
    Kappasorb->AddElement(nist->FindOrBuildElement("Na"), 1);
    Kappasorb->AddElement(nist->FindOrBuildElement("O"), 2);

    // Petrischale gefüllt mit Wasser und Kappasorb
    G4double density_kappasorbwater = (75.0 / 71125.658) * g/mm3; // Schüttdichte
    G4Material *KappasorbWater = new G4Material("KappasorbWater", density_kappasorbwater, 2);
    G4double waterFraction = ((93.4 - 1.6) / 93.4); // In der Petrischale sind 72.6 g Wasser
    G4double kappaFraction = (1.6 / 93.4); // In der Petrischale sind 1.6 g Kappasorb
    KappasorbWater->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER"), waterFraction);
    KappasorbWater->AddMaterial(Kappasorb, kappaFraction);

    // Handschuhe aus Nitrilkautschuk
    // CAS-Nr.: 9003-18-3 Poly(acrylonitril-co-butadien) 
    G4double density_nitril = (21.8 / 71125.658) * g/mm3; // Schüttdichte
    G4Material* Nitril = new G4Material("Nitril", density_nitril, 3);
    Nitril->AddElement(nist->FindOrBuildElement("C"), 7);
    Nitril->AddElement(nist->FindOrBuildElement("H"), 9);
    Nitril->AddElement(nist->FindOrBuildElement("N"), 1);

    // Aluminiumstück
    G4double density_al = (187.4 / 71125.658) * g/mm3; // Schüttdichte
    //G4double density_al = (187.4 / 67757.409) * g/mm3; // Schüttdichte
    G4Material* Al = nist->BuildMaterialWithNewDensity("G4_Al_new","G4_Al",density_al);

    /*********************************************************************************************************/

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    //if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection") {
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "../utr/DetectorConstruction/Others/Efficiency/Volumes/Petrischale_Fuellvolumen.gdml";
    }
    //else if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection/build") {
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/Petrischale_Fuellvolumen.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    Fuelvolumen_Logical = gdmlWorld->GetLogicalVolume();
    
    Fuelvolumen_Logical->SetVisAttributes(new G4VisAttributes(magenta));
    Fuelvolumen_Logical->SetMaterial({{material}}); 
   
    rot = new G4RotationMatrix();
  }

  ~Fuelvolumen(){};

  // placing the ProbenhalterKurz in the detector world with Put() methode
  // use ProbenhalterKurz *probenhalterKurz = new ProbenhalterKurz(world_logical) and probenhalterKurz->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), Fuelvolumen_Logical,
                      "Fuelvolumen", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), Fuelvolumen_Logical,
                      "Fuelvolumen", World_Logical, false, 0);
  }
};

class Folienhalterung {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *Folienhalterung_Logical;

  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Folienhalterung(G4LogicalVolume *world_Logical) {

    // color of Hohlzylinder_GEM
    G4Colour grey(0.5, 0.5, 0.5);

    World_Logical = world_Logical;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    //if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection") {
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "../utr/DetectorConstruction/Others/Efficiency/Volumes/Folienhalterung.gdml";
    }
    //else if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection/build") {
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/Folienhalterung.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    Folienhalterung_Logical = gdmlWorld->GetLogicalVolume();
    
    Folienhalterung_Logical->SetVisAttributes(new G4VisAttributes(grey));
    Folienhalterung_Logical->SetMaterial(polyethylene);
   
    rot = new G4RotationMatrix();
  }

  ~Folienhalterung(){};

  // placing the ProbenhalterKurz in the detector world with Put() methode
  // use ProbenhalterKurz *probenhalterKurz = new ProbenhalterKurz(world_logical) and probenhalterKurz->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), Folienhalterung_Logical,
                      "Folienhalterung", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), Folienhalterung_Logical,
                      "Folienhalterung", World_Logical, false, 0);
  }
};

class Zylinderhalterung {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *Zylinderhalterung_Logical;

  G4RotationMatrix *rot;
  G4GDMLParser parser;

  public:
  G4String gdmlFileName;

  Zylinderhalterung(G4LogicalVolume *world_Logical) {

    // color of Hohlzylinder_GEM
    G4Colour grey(0.5, 0.5, 0.5);

    World_Logical = world_Logical;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathString = currentPath.string();
    //if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection") {
    if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr") {
      gdmlFileName = "../utr/DetectorConstruction/Others/Efficiency/Volumes/Zylinderhalterung.gdml";
    }
    //else if (currentPathString == "/home/drichter/HOME/local/utr-radiationprotection/build") {
    else if (currentPathString == "/nfs/ldas02/zh02/home/drichter/utr/build") {
      gdmlFileName = "../DetectorConstruction/Others/Efficiency/Volumes/Zylinderhalterung.gdml";
    } 

    parser.Read(gdmlFileName);
    G4VPhysicalVolume* gdmlWorld = parser.GetWorldVolume();

    Zylinderhalterung_Logical = gdmlWorld->GetLogicalVolume();
    
    Zylinderhalterung_Logical->SetVisAttributes(new G4VisAttributes(grey));
    Zylinderhalterung_Logical->SetMaterial(polyethylene);
   
    rot = new G4RotationMatrix();
  }

  ~Zylinderhalterung(){};

  // placing the ProbenhalterKurz in the detector world with Put() methode
  // use ProbenhalterKurz *probenhalterKurz = new ProbenhalterKurz(world_logical) and probenhalterKurz->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), Zylinderhalterung_Logical,
                      "Zylinderhalterung", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), Zylinderhalterung_Logical,
                      "Zylinderhalterung", World_Logical, false, 0);
  }
};