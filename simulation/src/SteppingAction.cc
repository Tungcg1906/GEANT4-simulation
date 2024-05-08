//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4VProcess.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "math.h"
using namespace B4;

namespace B4a
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
                               EventAction* eventAction)
  : fDetConstruction(detConstruction),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{	
  // get event ID
  auto evtID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();


  G4double pid=0;

  //outgoing particle
  G4double tr=0;
  G4double pt=0;
  G4Track* track = step->GetTrack(); 

  // Collect energy and track length step by step
  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume(); // Pre!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  auto deltae = step->GetDeltaEnergy();

  auto threshold_edep = 0.001*CLHEP::MeV;
  auto threshold_delta = 0.1*CLHEP::MeV;
  if(edep>threshold_edep || abs(deltae)>threshold_delta){    // Cut energy deposit > 1keV and cut delta e > 100keV

    //###############################################################################
    // part_info tree

    // get tracks info

 	  tr=track->GetTrackID();
  	pt=track->GetParentID();

  	out_pno=out_pno+1;


    // get particle ID

    const G4DynamicParticle* dynParticle = track->GetDynamicParticle();
    G4ParticleDefinition* particle = dynParticle -> GetDefinition();
    G4double part_id = particle->GetPDGEncoding();


    // compute momenta

    G4double pxo=step->GetPostStepPoint()->GetMomentum().x();
    G4double pyo=step->GetPostStepPoint()->GetMomentum().y();
    G4double pzo=step->GetPostStepPoint()->GetMomentum().z();
    G4double po=sqrt(pow(pzo,2)+pow(pyo,2)+pow(pxo,2)); // Calculate mom


    // compute geometric and temporal info

    auto touchable = step->GetPreStepPoint()->GetTouchable(); // Pre!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    G4int picell_idx = touchable->GetVolume()->GetCopyNo();

    G4int z_idx = picell_idx / 100000;
    G4int x_idx = ((picell_idx % 10000) % 100) / 10;
    G4int y_idx = (picell_idx % 10000) / 1000;
    G4double picublet_idx = z_idx * 100 + y_idx * 10 + x_idx;

    G4int z_idx_cub = (picell_idx / 10000) % 10;
    G4int x_idx_cub = ((picell_idx % 10000) % 100) % 10;
    G4int y_idx_cub = ((picell_idx % 10000) / 100) % 10;
    G4double picells_in_cublet_idx = z_idx_cub * 100 + y_idx_cub * 10 + x_idx_cub;
        
    G4double glb_t=step->GetPostStepPoint()->GetGlobalTime();


	  // Filling MC truth ntuple

  	auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0,0,evtID);
  	analysisManager->FillNtupleDColumn(0,1,part_id);
  	analysisManager->FillNtupleDColumn(0,2,tr);
  	analysisManager->FillNtupleDColumn(0,3,pt);
  	analysisManager->FillNtupleDColumn(0,4,po); // mom
    analysisManager->FillNtupleDColumn(0,5,edep);
    analysisManager->FillNtupleDColumn(0,6,deltae);
  	analysisManager->FillNtupleDColumn(0,7,glb_t);
    analysisManager->FillNtupleDColumn(0,8,picublet_idx);
    analysisManager->FillNtupleDColumn(0,9,picells_in_cublet_idx);
    // analysisManager->FillNtupleDColumn(0,9,pos_x);
  	// analysisManager->FillNtupleDColumn(0,10,pos_y);
  	// analysisManager->FillNtupleDColumn(0,11,pos_z);
  	// analysisManager->FillNtupleDColumn(0,12,loc_t);
    // analysisManager->FillNtupleDColumn(0,13,picell_idx);

	  // All information filled. Add row to ntuple
  	analysisManager->AddNtupleRow(0);
    tr=track->GetTrackID();
  	pt=track->GetParentID();
  	pid=part_id;
  	trid=tr;

  }

  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  if ( volume == fDetConstruction->GetAbsorberPV() ) {
    fEventAction->AddAbs(edep,stepLength);
  }

  if (track->GetTrackID()!=1) {
  	tr=track->GetTrackID();
  	pt=track->GetParentID();
  }
  
  if (track->GetTrackID()== 1) {
 	  tr=track->GetTrackID();
  	pt=track->GetParentID();
  }

  if ( volume == fDetConstruction->GetGapPV() ) {
    fEventAction->AddGap(edep,stepLength);
  }

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
