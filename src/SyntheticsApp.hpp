//----------------------------------------------------------------------------
// The MIT License
// Took the basics from Polycode Examples
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_APP_H
#define SYNTHETICS_APP_H

#include <Polycode.h>
#include "PolycodeView.h"
#include "Polycode3DPhysics.h"

#include "DisplayKeyHits.hpp"
#include "Constructor/Display.hpp"
#include "Configurator.hpp"

namespace Synthetics {
  class SyntheticsApp {
    public:
      SyntheticsApp(Polycode::PolycodeView *view);
      ~SyntheticsApp();
      void handleEvent(Polycode::Event *e);
      bool Update();

    private:
      Polycode::Core *m_core;
      Configurator *m_conf;
      DisplayKeyHits *m_displayKeyHits;
      Constructor::Display *m_constructor;
  };
}
#endif
