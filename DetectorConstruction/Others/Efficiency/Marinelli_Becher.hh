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
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// Define adaptor for Eurysis Mesures HPGe detector in the isotop laboratory.
// Is placed on top of the detector end cap.
// The adapter was printed with a 3D printer.
// The exact material has yet to be found out. Assumption: PLA (polylactic acid)) 

class MarinelliBeaker {
  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *MarinelliBeaker_Logical;
  G4Cons *OuterCone_Solid;
  G4Cons *InnerCone_Solid;
  G4Tubs *OuterTube_Solid;
  G4Tubs *InnerTube_Solid;
  G4SubtractionSolid *Cone_Solid;
  G4UnionSolid *ConeTube_Solid;
  G4SubtractionSolid *MarinelliBeaker_Solid;
  G4RotationMatrix *rot;

  public:
  // OuterCone
  G4double OC_ir_p; // inner radius plus (bigger diameter)
  G4double OC_or_p; // outer radius plus (bigger diameter)
  G4double OC_ir_m; // inner radius minus (smaller diameter)
  G4double OC_or_m; // outer radius minus (smaller diameter)
  G4double OC_zhl; // half length on z axis
  // InnerCone
  G4double IC_ir_p; // inner radius plus (bigger diameter)
  G4double IC_or_p; // outer radius plus (bigger diameter)
  G4double IC_ir_m; // inner radius minus (smaller diameter)
  G4double IC_or_m; // outer radius minus (smaller diameter)
  G4double IC_zhl; // half length on z axis
  // OuterTube
  G4double OT_ir; // inner radius
  G4double OT_or; // outer radius
  G4double OT_zhl; // half length on z axis
  // InnerTube
  G4double IT_ir; // inner radius
  G4double IT_or; // outer radius
  G4double IT_zhl; // half length on z axis
  // for all geoemtries
  G4double sphi; // start angle
  G4double dphi; // full angle

  MarinelliBeaker(G4LogicalVolume *world_Logical) {
    // dimensions of marinelli
    // Vorsicht: Deckel ragt ca. 8 mm in den Becher rein

    // OuterCone
    OC_ir_p = 0.5 * 0. * mm;
    OC_or_p = 0.5 * 201. * mm; // Hersteller: 201 mm
    OC_ir_m = 0.5 * 0. * mm; 
    OC_or_m = 0.5 * 188. * mm; // gemessen: 188 mm
    OC_zhl = 0.5 * 178. * mm; // Hersteller: 178 mm

    // InnerCone -> Berücksichtigen der 3 mm Dicke der Außenwände
    IC_ir_p = 0.5 * 0. * mm;
    IC_or_p = 0.5 * 195. * mm;
    IC_ir_m = 0.5 * 0. * mm;
    IC_or_m = 0.5 * 182. * mm;
    IC_zhl = 0.5 * 175.1 * mm;

    // OuterTube
    OT_ir = 0.5 * 0. * mm;
    OT_or = 0.5 * 102. * mm; // gemessen: 100 mm
    OT_zhl = 0.5 * 102. * mm; // Hersteller: 102 mm

    // InnerTube -> Berücksichtigen der 3 mm Dicke der Außenwände
    IT_ir = 0.5 * 0. * mm;
    IT_or = 0.5 * 96. * mm; // Hersteller: 96 mm
    IT_zhl = 0.5 * 99. * mm;


    // for all geometries
    sphi = 0. * deg;
    dphi = 360. * deg;


    // color of marinelli
    G4Colour magenta(1.0, 0.0, 1.0);

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polypropylene = nist->FindOrBuildMaterial("G4_POLYPROPYLENE");

    World_Logical = world_Logical;

    OuterCone_Solid = new G4Cons("OuterCone_Solid", OC_ir_p, OC_or_p, OC_ir_m, OC_or_m, OC_zhl, sphi, dphi);
    InnerCone_Solid = new G4Cons("InnerCone_Solid", IC_ir_p, IC_or_p, IC_ir_m, IC_or_m, IC_zhl, sphi, dphi);
    OuterTube_Solid = new G4Tubs("OuterTube_Solid", OT_ir, OT_or, OT_zhl, sphi, dphi);
    InnerTube_Solid = new G4Tubs("InnerTube_Solid", IT_ir, IT_or, IT_zhl, sphi, dphi);
    
    
    // Create solid for Marinelli Beaker
    G4ThreeVector setCones(0.,0.,-1.5);
    Cone_Solid = new G4SubtractionSolid("Cone_Solid", OuterCone_Solid, InnerCone_Solid, 0, setCones);
    G4ThreeVector mergewithOT(0.,0.,38.);
    ConeTube_Solid = new G4UnionSolid("ConeTube_Solid", Cone_Solid, OuterTube_Solid, 0, mergewithOT);
    G4ThreeVector mergetoBeaker(0.,0.,39.5);
    MarinelliBeaker_Solid = new G4SubtractionSolid("MarinelliBeaker_Solid", ConeTube_Solid, InnerTube_Solid, 0, mergetoBeaker);

    MarinelliBeaker_Logical = new G4LogicalVolume(MarinelliBeaker_Solid, polypropylene, "MarinelliBeaker_Logical", 0, 0, 0);
    
    MarinelliBeaker_Logical->SetVisAttributes(new G4VisAttributes(magenta));
   
    rot = new G4RotationMatrix();
  }

  ~MarinelliBeaker(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), MarinelliBeaker_Logical,
                      "MarinelliBeaker", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), MarinelliBeaker_Logical,
                      "MarinelliBeaker", World_Logical, false, 0);
  }
};

class MarinelliDeckel {
  private:
  G4LogicalVolume *World_Logical;
  G4Tubs *DeckelOben_Solid;
  G4Cons *DeckelUnten_Solid;
  G4UnionSolid *MarinelliDeckel_Solid;
  G4LogicalVolume *MarinelliDeckel_Logical;
  G4RotationMatrix *rot;

  public:
  // Deckel oben
  G4double TD_ir; // inner radius
  G4double TD_or; // outer radius
  G4double TD_zhl; // half length on z axis
  // Deckel unten
  G4double CD_ir_p; // inner radius plus (bigger diameter)
  G4double CD_or_p; // outer radius plus (bigger diameter)
  G4double CD_ir_m; // inner radius minus (smaller diameter)
  G4double CD_or_m; // outer radius minus (smaller diameter)
  G4double CD_zhl; // half length on z axis
  // for all geometries
  G4double sphi;
  G4double dphi;

  MarinelliDeckel(G4LogicalVolume *world_Logical) {
    // dimensions of Marinelli Deckel
    // Deckel oben
    TD_ir = 0.5 * 0. * mm;
    TD_or = 0.5 * 201. * mm;
    TD_zhl = 0.5 * 5. * mm;
    // Deckel unten
    CD_ir_p = 0.5 * 0. * mm;
    CD_or_p = 0.5 * 195. * mm;
    CD_ir_m = 0.5 * 0. * mm;
    CD_or_m = 0.5 * 194.4057143 * mm; // mit Dreisatz berechnet
    CD_zhl = 0.5 * 8. * mm;
    // for all geometries
    sphi = 0. * deg;
    dphi = 360. * deg;

    // color of Marinelli Deckel
    G4Colour magenta(1.0, 0.0, 1.0);

    // define materials polypropylene an polyethylene (deutsch: Polypropylen, Polyethylen)
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    World_Logical = world_Logical;

    DeckelOben_Solid = new G4Tubs("DeckelOben_Solid", TD_ir, TD_or, TD_zhl, sphi, dphi);
    DeckelUnten_Solid = new G4Cons("DeckelUnten_Solid", CD_ir_p, CD_or_p, CD_ir_m, CD_or_m, CD_zhl, sphi, dphi);

    // Create solid for Marinelli Deckel
    G4ThreeVector merge(0.,0.,6.5);
    MarinelliDeckel_Solid = new G4UnionSolid("MarinelliDeckel_Solid", DeckelOben_Solid, DeckelUnten_Solid, 0, merge);

    // Deckel
    MarinelliDeckel_Logical = new G4LogicalVolume(MarinelliDeckel_Solid, polyethylene, "MarinelliDeckel_Logical", 0, 0, 0);

    MarinelliDeckel_Logical->SetVisAttributes(new G4VisAttributes(magenta));
   
    rot = new G4RotationMatrix();
  }

  ~MarinelliDeckel(){};

  // placing the adaptor in the detector world with Put() methode
  // use Marinelli *marinelli = new Marinelli(world_logical) and marinelli->Put(0., 0., 90.)
  void Put(G4double x, G4double y, G4double z) {
    new G4PVPlacement(0, G4ThreeVector(x, y, z), MarinelliDeckel_Logical,
                      "MarinelliDeckel", World_Logical, false, 0);
  }

  void Put(G4double x, G4double y, G4double z, G4double angle_x,
           G4double angle_y, G4double angle_z) {

    rot = new G4RotationMatrix();
    rot->rotateX(angle_x);
    rot->rotateY(angle_y);
    rot->rotateZ(angle_z);

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), MarinelliDeckel_Logical,
                      "MarinelliDeckel", World_Logical, false, 0);
  }
};
