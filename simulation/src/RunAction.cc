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
/// \file RunAction.cc
/// \brief Implementation of the B4::RunAction class


#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B4
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  G4String fileName = "hadron.root";
  analysisManager->OpenFile(fileName);
  
  analysisManager->CreateNtuple("part_info","part_info");
  analysisManager->CreateNtupleDColumn("event_id");
  analysisManager->CreateNtupleDColumn("part_id");
  analysisManager->CreateNtupleDColumn("track_id");
  analysisManager->CreateNtupleDColumn("parent_id");
  analysisManager->CreateNtupleDColumn("mom");
  analysisManager->CreateNtupleDColumn("edepo");
  analysisManager->CreateNtupleDColumn("global_t");
  analysisManager->CreateNtupleDColumn("cublet_idx");
  analysisManager->CreateNtupleDColumn("cell_in_cub");
  // analysisManager->CreateNtupleDColumn("pos_x");
  // analysisManager->CreateNtupleDColumn("pos_y");
  // analysisManager->CreateNtupleDColumn("pos_z");
  // analysisManager->CreateNtupleDColumn("local_t");
  // analysisManager->CreateNtupleDColumn("picell_idx");
  analysisManager->FinishNtuple(0);
  
  // analysisManager->CreateNtuple("Edep","Edep");
  // analysisManager->CreateNtupleDColumn("Event_ID");
  // analysisManager->CreateNtupleDColumn("edep");
  // analysisManager->CreateNtupleDColumn("cell_idx");
  // analysisManager->CreateNtupleDColumn("cublet_idx");
  // analysisManager->CreateNtupleDColumn("cells_in_cublet_idx");
  // analysisManager->FinishNtuple(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  
  // Open an output file
  G4String fileName = "hadron.root";
  analysisManager->OpenFile(fileName); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  //
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}


