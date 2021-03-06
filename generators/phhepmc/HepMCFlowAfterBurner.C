//
// Inspired by code from ATLAS.  Thanks!
//
#include "HepMCFlowAfterBurner.h"

#include <PHHepMCGenEvent.h>

#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <phool.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <memory>

#include <HepMC/GenEvent.h>

#include <CLHEP/Random/RandomEngine.h>
#include <CLHEP/Random/MTwistEngine.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <CLHEP/Geometry/Point3D.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <flowAfterburner.h>

using namespace std;

CLHEP::HepRandomEngine * engine = NULL;

HepMCFlowAfterBurner::HepMCFlowAfterBurner(const std::string &name):
  SubsysReco(name),
  config_filename("flowAfterburner.xml"),
  algorithmName("JJNEW"),
  mineta(-1),
  maxeta(-1),
  mineta_macro(NAN),
  maxeta_macro(NAN),
  minpt(0.),
  maxpt(100.),
  minpt_macro(NAN),
  maxpt_macro(NAN),
  seedset(0),
  seed(0),
  randomSeed(11793)
{}


int
HepMCFlowAfterBurner::Init(PHCompositeNode *topNode)
{

  using boost::property_tree::ptree;
  ptree pt;

  std::ifstream config_file(config_filename.c_str());

  if (config_file)
    {
      // Read XML configuration file.
      read_xml (config_file, pt);
    }
  if (seedset)
    {
      randomSeed = seed;
    }
  else
    {
      randomSeed = pt.get ("FLOWAFTERBURNER.RANDOM.SEED", randomSeed);
    }

  engine = new CLHEP::MTwistEngine (randomSeed);

  if (isfinite(mineta_macro))
    {
      mineta = mineta_macro;
    }
  else
    {
      mineta = pt.get("FLOWAFTERBURNER.CUTS.MINETA", mineta);
    }
  if (isfinite(maxeta_macro))
    {
      maxeta = maxeta_macro;
    }
  else
    {
      maxeta = pt.get("FLOWAFTERBURNER.CUTS.MAXETA", maxeta);
    }
  if (isfinite(minpt_macro))
    {
      minpt = minpt_macro;
    }
  else
    {
      minpt = pt.get("FLOWAFTERBURNER.CUTS.MINPT", minpt);
    }

  if (isfinite(maxpt_macro))
    {
      maxpt = maxpt_macro;
    }
  else
    {
      maxpt = pt.get("FLOWAFTERBURNER.CUTS.MAXPT", maxpt);
    }

  if (algorithmName_macro.size() > 0)
    {
      algorithmName = algorithmName_macro;
    }
  else
    {
      algorithmName = pt.get("FLOWAFTERBURNER.ALGORITHM", algorithmName);
    }

  return 0;
}

int
HepMCFlowAfterBurner::process_event(PHCompositeNode *topNode)
{
  PHHepMCGenEvent *genevt = findNode::getClass<PHHepMCGenEvent>(topNode,"PHHepMCGenEvent");
  HepMC::GenEvent *evt =  genevt->getEvent();
  if (!evt)
    {
      cout << PHWHERE << " no evt pointer under HEPMC Node found" << endl;
      return ABORTEVENT;
    }
  if (verbosity > 0)
    {
      cout << "calling flowAfterburner with algorithm "
           << algorithmName << ", mineta " << mineta
           << ", maxeta: " << maxeta << ", minpt: " << minpt
           << ", maxpt: " << maxpt << endl;
    }
  flowAfterburner(evt, engine, algorithmName, mineta, maxeta, minpt, maxpt);
  return EVENT_OK;
}

void
HepMCFlowAfterBurner::setSeed(const long il)
{
  seedset = 1;
  seed = il;
  randomSeed = seed;
  // just in case we are already running, kill the engine and make
  // a new one using the selected seed
  if (engine)
    {
      delete engine;
      engine = new CLHEP::MTwistEngine (randomSeed);
    }
  return;
}

void
HepMCFlowAfterBurner::SaveRandomState(const string &savefile)
{
  if (engine)
    {
      engine->saveStatus(savefile.c_str());
      return;
    }
  cout << PHWHERE << " Radom engine not started yet" << endl;
}

void
HepMCFlowAfterBurner::RestoreRandomState(const string &savefile)
{
  if (engine)
    {
      engine->restoreStatus(savefile.c_str());
      return;
    }
  cout << PHWHERE << " Radom engine not started yet" << endl;
}
