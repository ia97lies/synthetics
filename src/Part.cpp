//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#include "PolyEntity.h"
#include "Part.hpp"

namespace Synthetics {
  Part::Part() {};

  Part::~Part() {};

  int Part::getNoPlugs() {
    return m_plugs.size();
  }

  void Part::addPlug(Plug *plug) {
    m_plugs.push_back(plug);
  }

  Plug *Part::getPlug(int i) {
    return m_plugs.at(i);
  }

  Plug *Part::getPlug(Polycode::Entity *plugShape) {
    return NULL;
  }

  Polycode::Entity *getShape() {
    return NULL;
  }
}
