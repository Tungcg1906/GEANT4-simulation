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
 	
  G4double mom=0;
  G4double pid=0;
  
  G4Track* track = step->GetTrack();
  
  auto pdgid = track->GetDefinition()->GetPDGEncoding();	  
  const G4DynamicParticle* dynParticle = track->GetDynamicParticle();
  G4ParticleDefinition* particle = dynParticle -> GetDefinition();
  G4double kinEnergy = dynParticle-> GetKineticEnergy();
  G4String particleName = particle-> GetParticleName();

  //outgoing particle
  G4double tr=0;
  G4double pt=0;
  G4double pxo=step->GetPostStepPoint()->GetMomentum().x();
  G4double pyo=step->GetPostStepPoint()->GetMomentum().y();
  G4double pzo=step->GetPostStepPoint()->GetMomentum().z();
  G4double pos_x=step->GetPostStepPoint()->GetPosition().x();
  G4double pos_y=step->GetPostStepPoint()->GetPosition().y();
  G4double pos_z=step->GetPostStepPoint()->GetPosition().z();
  G4double loc_t=step->GetPostStepPoint()->GetLocalTime();
  G4double glb_t=step->GetPostStepPoint()->GetGlobalTime();
  G4double po=sqrt(pow(pzo,2)+pow(pyo,2)+pow(pxo,2));
  G4int ix = (pos_x+14.5)/3;
  G4int iy = (pos_y+14.5)/3;
  G4int iz = (pos_z+59.5)/12;
  G4int picell_idx = ix + 10*iy + 100*iz;
  G4int z_idx = picell_idx/200;
  G4int x_idx = ((picell_idx%100)%10)/2;
  G4int y_idx = (picell_idx%100)/200;
  G4double picublet_idx = z_idx*25 + y_idx*5 + x_idx;
  G4double picells_in_cublet = x_idx - (x_idx /2)*2 + (y_idx - (y_idx/2)*2)*10 + (z_idx - (z_idx/2)*2)*10;

  if(po>1.*CLHEP::MeV){       // ENERGY CUT FOR DEBUGGING!!!!! <<< ---------------
  	tr=track->GetTrackID();
  	pt=track->GetParentID();
  	// if(trid!=tr){  // Commented. Not sure why Rumman put it
  	out_pno=out_pno+1;
  	G4double part_id = particle->GetPDGEncoding();

	  // Filling MC truth ntuple
    // Get event ID and fill ntuple
	  auto evtID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  	auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0,0,evtID);
  	analysisManager->FillNtupleDColumn(0,1,part_id);
  	analysisManager->FillNtupleDColumn(0,2,tr);
  	analysisManager->FillNtupleDColumn(0,3,pt);
    analysisManager->FillNtupleDColumn(0,4,pos_x);
  	analysisManager->FillNtupleDColumn(0,5,pos_y);
  	analysisManager->FillNtupleDColumn(0,6,pos_z);
  	analysisManager->FillNtupleDColumn(0,7,po);
  	analysisManager->FillNtupleDColumn(0,8,loc_t);
  	analysisManager->FillNtupleDColumn(0,9,glb_t);
    analysisManager->FillNtupleDColumn(0,10,picell_idx);
    analysisManager->FillNtupleDColumn(0,11,picublet_idx);
    analysisManager->FillNtupleDColumn(0,12,picells_in_cublet);

	  // All information filled. Add row to ntuple
  	analysisManager->AddNtupleRow(0);
  	G4cout<<  " EVENT N. :" <<evtID<<"\n";
  	pid=part_id;
  	tr=track->GetTrackID();
  	pt=track->GetParentID();
  	trid=tr;
  	// }
  }
  
  // Collect energy and track length step by step

  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  tot= tot+edep;
  if(edep>0){
	// Fill cell ntuple with energy deposit
    auto analysisManager = G4AnalysisManager::Instance();
    auto touchable = step->GetPreStepPoint()->GetTouchable();
    G4int cell_idx=touchable->GetVolume()->GetCopyNo();
    G4int z_idx = cell_idx/200;
    G4int x_idx = ((cell_idx%100)%10)/2;
    G4int y_idx = (cell_idx%100)/200;
    G4double cublet_idx = z_idx*25 + y_idx*5 + x_idx;
    G4double cells_in_cublet = x_idx - (x_idx /2)*2 + (y_idx - (y_idx/2)*2)*10 + (z_idx - (z_idx/2)*2)*10;
    //G4int y_idx = (cell_idx - z_idx * 200)/ 2;
    //G4int x_idx = cell_idx - z_idx * 200 - y_idx * 2;
    //G4double cublet_idx = x_idx / 2 + (y_idx / 2) * 2 + (z_idx / 2) * 200;

    // Get event ID and fill ntuple
	  auto evtID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	  analysisManager->FillNtupleDColumn(1,0,evtID);
    analysisManager->FillNtupleDColumn(1,1,edep);
    analysisManager->FillNtupleDColumn(1,2,cell_idx);
    analysisManager->FillNtupleDColumn(1,3,cublet_idx);
    analysisManager->FillNtupleDColumn(1,4,cells_in_cublet);
    
    // All information filled. Add row to ntuple
    analysisManager->AddNtupleRow(1);	

;
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
