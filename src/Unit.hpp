//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_UNIT_H
#define SYNTHETICS_UNIT_H

#include <Polycode.h>
#include "PolycodeView.h"

namespace Synthetics {

  class Unit : public ::Polycode::EventHandler {
    public:
      virtual ~Unit() {};

      virtual int noOfFaces() = 0;
      virtual ::Polycode::Vector3 getOrientation(int face) = 0;
      virtual ::std::string getName(int face) = 0;
      virtual void addUnit(int face, Unit *unit) = 0;
      virtual Unit *getUnit(int face) = 0;
      virtual void handleEvent(::Polycode::Event *event);
  };

}

#endif