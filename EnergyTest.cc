// Scorer for EnergyTest
//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * 								      *
// * 		Extension scorer in TOPAS to calculate physical       *
// *         parameters of interest for each interaction	      *
// *                                                                  *
// *                                                                  *
// ********************************************************************
//

#include "EnergyTest.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"

EnergyTest::EnergyTest(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
						  G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer)
						 : TsVBinnedScorer(pM, mM, gM, scM, eM, scorerName, quantity, outFileName, isSubScorer)
{
	SetUnit("MeV"); 
}


EnergyTest::~EnergyTest() {;}


G4bool EnergyTest::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
	if (!fIsActive) {
	      	fSkippedWhileInactive++;
		return false;
	}
	
        
  const G4Track* track = aStep->GetTrack();  
  G4double e = track->GetKineticEnergy();
  G4StepPoint* point1 = aStep->GetPreStepPoint();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  G4double PreEnergy  = point1->GetTotalEnergy();
  G4int prec=G4cout.precision(10); 
  const G4VProcess* process 
                      = aStep->GetPostStepPoint()->GetProcessDefinedStep();    
  G4String procName = " UserLimit";
	
  if (process) procName = process->GetProcessName();
	G4double edep2 = aStep->GetTotalEnergyDeposit();

	if ( edep2 >= 0){ // only if the energy deposited in the material is higher than 0
	    G4double edep =edep2* 2. * aStep->GetPreStepPoint()->GetWeight();  // weight the energy deposited
	    AccumulateHit(aStep, edep);  
      	    const std::vector<const G4Track*>* secondary 
                                    = aStep->GetSecondaryInCurrentStep();
	    size_t nbtrk = (*secondary).size();  // size of array of secondary particles 

	    if (nbtrk ) {    // only if secondary particles are created: print data

		G4cout <<"process= " <<procName << G4endl;
                G4cout <<"Initial Energy= " <<PreEnergy/keV << G4endl;
                G4cout <<"Final Energy= " <<PostEnergy/keV << G4endl;
                G4cout <<"Energy deposited = " <<edep2/keV << G4endl;

		G4cout << "\n    :----- List of secondaries ----------------" << G4endl;
                G4cout << "number of secondaries= "<<nbtrk    << G4endl;

		for (size_t lp=0; lp<(*secondary).size(); lp++) {
			G4cout << "   "
			 << (*secondary)[lp]->GetDefinition()->GetParticleName()
			 << ":  energy = "
			 << ((*secondary)[lp]->GetKineticEnergy())/keV;
		   	G4cout << G4endl;

		   	G4cout <<"Local Time(ps)= " << ((*secondary)[lp]->GetLocalTime())/ps << G4endl;
		        G4cout <<"Global Time(ps)= " << ((*secondary)[lp]->GetGlobalTime())/ps << G4endl;

           
		}
		 
	  }

	  return true;
	}
	return false;	
}
