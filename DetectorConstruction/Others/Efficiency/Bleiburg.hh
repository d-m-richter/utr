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

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//#include "DetectorConstruction.hh"

/*********************** Aluminumgehäuse ****************************/

class AluFront_out {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *AluFront_Logical;
  G4Box *AluFront_Solid;
  G4RotationMatrix *rot;

  G4double AluFront_medium;
  G4double AluFront_short;
  G4double AluFront_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  AluFront_out(G4LogicalVolume *world_Logical) {
    L = 400.3 * mm;
    M = 10. * mm;
    S = 360. * mm;

    G4Colour green(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    World_Logical = world_Logical;

    AluFront_Solid = new G4Box("AluFront_Solid", M / 2., S / 2., L / 2.);
    AluFront_Logical = new G4LogicalVolume(AluFront_Solid, Al,
                                            "AluFront_Logical", 0, 0, 0);

    AluFront_Logical->SetVisAttributes(new G4VisAttributes(green));

    rot = new G4RotationMatrix();
  }

  ~AluFront_out(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), AluFront_Logical,
                      "AluFront", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), AluFront_Logical,
                      "AluFront", World_Logical, false, 0);
  }
};

class AluSide_out {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *AluSide_Logical;
  G4Box *AluSide_Solid;
  G4RotationMatrix *rot;

  G4double AluSide_medium;
  G4double AluSide_short;
  G4double AluSide_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  AluSide_out(G4LogicalVolume *world_Logical) {
    L = 400.3 * mm;
    M = 10. * mm;
    S = 375. * mm;

    G4Colour green(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    World_Logical = world_Logical;

    AluSide_Solid = new G4Box("AluSide_Solid", M / 2., S / 2., L / 2.);
    AluSide_Logical = new G4LogicalVolume(AluSide_Solid, Al,
                                            "AluSide_Logical", 0, 0, 0);

    AluSide_Logical->SetVisAttributes(new G4VisAttributes(green));

    rot = new G4RotationMatrix();
  }

  ~AluSide_out(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), AluSide_Logical,
                      "AluSide", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), AluSide_Logical,
                      "AluSide", World_Logical, false, 0);
  }
};

class AluFront_in {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *AluFront_Logical;
  G4Box *AluFront_Solid;
  G4RotationMatrix *rot;

  G4double AluFront_medium;
  G4double AluFront_short;
  G4double AluFront_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  AluFront_in(G4LogicalVolume *world_Logical) {
    L = 400.3 * mm;
    M = 10. * mm;
    S = 227.5 * mm;

    G4Colour green(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    World_Logical = world_Logical;

    AluFront_Solid = new G4Box("AluFront_Solid", M / 2., S / 2., L / 2.);
    AluFront_Logical = new G4LogicalVolume(AluFront_Solid, Al,
                                            "AluFront_Logical", 0, 0, 0);

    AluFront_Logical->SetVisAttributes(new G4VisAttributes(green));

    rot = new G4RotationMatrix();
  }

  ~AluFront_in(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), AluFront_Logical,
                      "AluFront", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), AluFront_Logical,
                      "AluFront", World_Logical, false, 0);
  }
};

class AluSide_in {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *AluSide_Logical;
  G4Box *AluSide_Solid;
  G4RotationMatrix *rot;

  G4double AluSide_medium;
  G4double AluSide_short;
  G4double AluSide_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  AluSide_in(G4LogicalVolume *world_Logical) {
    L = 400.3 * mm;
    M = 10. * mm;
    S = 247.5 * mm;

    G4Colour green(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    World_Logical = world_Logical;

    AluSide_Solid = new G4Box("AluSide_Solid", M / 2., S / 2., L / 2.);
    AluSide_Logical = new G4LogicalVolume(AluSide_Solid, Al,
                                            "AluSide_Logical", 0, 0, 0);

    AluSide_Logical->SetVisAttributes(new G4VisAttributes(green));

    rot = new G4RotationMatrix();
  }

  ~AluSide_in(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), AluSide_Logical,
                      "AluSide", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), AluSide_Logical,
                      "AluSide", World_Logical, false, 0);
  }
};

class AluTop {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *AluTop_Logical;
  G4Box *AluTop_Solid;
  G4RotationMatrix *rot;

  G4double AluTop_medium;
  G4double AluTop_short;
  G4double AluTop_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  AluTop(G4LogicalVolume *world_Logical) {
    L = 360. * mm;
    M = 10. * mm;
    S = 360. * mm;

    G4Colour green(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    World_Logical = world_Logical;

    AluTop_Solid = new G4Box("AluTop_Solid", M / 2., S / 2., L / 2.);
    AluTop_Logical = new G4LogicalVolume(AluTop_Solid, Al,
                                            "AluTop_Logical", 0, 0, 0);

    AluTop_Logical->SetVisAttributes(new G4VisAttributes(green));

    rot = new G4RotationMatrix();
  }

  ~AluTop(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), AluTop_Logical,
                      "AluTop", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), AluTop_Logical,
                      "AluTop", World_Logical, false, 0);
  }
};

class AluBottom_withHole {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *AluBottom_Logical;
  G4Box *AluBox_Solid;
  G4Tubs *AluTube_Solid;
  G4SubtractionSolid *AluBottom_Solid;
  G4RotationMatrix *rot;

  G4double AluBottom_medium;
  G4double AluBottom_short;
  G4double AluBottom_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  G4double Rin;
  G4double Rout;
  G4double H;

  AluBottom_withHole(G4LogicalVolume *world_Logical) {
    L = 25. * mm;
    M = 225. * mm;
    S = 225. * mm;

    Rin = 0 *mm;
    Rout = 86. *mm;
    H = 25.1 *mm;

    G4Colour green(0., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    World_Logical = world_Logical;

    AluBox_Solid = new G4Box("AluBox_Solid", M / 2., S / 2., L / 2.);
    AluTube_Solid = new G4Tubs("Alu_Tube_Solid", Rin/2., Rout/2., H/2., 0. * deg, 360. * deg);
    G4ThreeVector move(0.,0.,0.);
    AluBottom_Solid = new G4SubtractionSolid("AluBottom_Solid", AluBox_Solid, AluTube_Solid, 0, move);
    AluBottom_Logical = new G4LogicalVolume(AluBottom_Solid, Al,
                                            "AluBottom_Logical", 0, 0, 0);

    AluBottom_Logical->SetVisAttributes(new G4VisAttributes(green));

    rot = new G4RotationMatrix();
  }

  ~AluBottom_withHole(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), AluBottom_Logical,
                      "AluBottom", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), AluBottom_Logical,
                      "AluBottom", World_Logical, false, 0);
  }
};

/********************* Bleiblöcke **************************/

class NormBrick {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *NormBrick_Logical;
  G4Box *NormBrick_Solid;
  G4RotationMatrix *rot;

  G4double NormBrick_medium;
  G4double NormBrick_short;
  G4double NormBrick_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  NormBrick(G4LogicalVolume *world_Logical) {
    L = 200. * mm;
    M = 100. * mm;
    S = 50. * mm;

    G4Colour yellow(1., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

    World_Logical = world_Logical;

    NormBrick_Solid = new G4Box("NormBrick_Solid", M / 2., S / 2., L / 2.);
    NormBrick_Logical = new G4LogicalVolume(NormBrick_Solid, Pb,
                                            "NormBrick_Logical", 0, 0, 0);

    NormBrick_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    rot = new G4RotationMatrix();
  }

  ~NormBrick(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), NormBrick_Logical,
                      "NormBrick", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), NormBrick_Logical,
                      "NormBrick", World_Logical, false, 0);
  }
};

class ShortNormBrick {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *ShortNormBrick_Logical;
  G4Box *ShortNormBrick_Solid;
  G4RotationMatrix *rot;

  G4double ShortNormBrick_medium;
  G4double ShortNormBrick_short;
  G4double ShortNormBrick_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  ShortNormBrick(G4LogicalVolume *world_Logical) {
    L = 100. * mm;
    M = 100. * mm;
    S = 50. * mm;

    G4Colour yellow(1., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

    World_Logical = world_Logical;

    ShortNormBrick_Solid =
        new G4Box("ShortNormBrick_Solid", M / 2., S / 2., L / 2.);
    ShortNormBrick_Logical = new G4LogicalVolume(
        ShortNormBrick_Solid, Pb, "ShortNormBrick_Logical", 0, 0, 0);

    ShortNormBrick_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    rot = new G4RotationMatrix();
  }

  ~ShortNormBrick(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), ShortNormBrick_Logical,
                      "ShortNormBrick", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), ShortNormBrick_Logical,
                      "ShortNormBrick", World_Logical, false, 0);
  }
};

class ShortShortNormBrick {
  private:
  G4LogicalVolume *World_Logical;

  G4LogicalVolume *ShortNormBrick_Logical;
  G4Box *ShortNormBrick_Solid;
  G4RotationMatrix *rot;

  G4double ShortNormBrick_medium;
  G4double ShortNormBrick_short;
  G4double ShortNormBrick_long;

  public:
  G4double L;
  G4double M;
  G4double S;

  ShortShortNormBrick(G4LogicalVolume *world_Logical) {
    L = 50. * mm;
    M = 100. * mm;
    S = 50. * mm;

    G4Colour yellow(1., 1., 0.);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

    World_Logical = world_Logical;

    ShortNormBrick_Solid =
        new G4Box("ShortNormBrick_Solid", M / 2., S / 2., L / 2.);
    ShortNormBrick_Logical = new G4LogicalVolume(
        ShortNormBrick_Solid, Pb, "ShortNormBrick_Logical", 0, 0, 0);

    ShortNormBrick_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    rot = new G4RotationMatrix();
  }

  ~ShortShortNormBrick(){};

  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), ShortNormBrick_Logical,
                      "ShortNormBrick", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), ShortNormBrick_Logical,
                      "ShortNormBrick", World_Logical, false, 0);
  }
};






