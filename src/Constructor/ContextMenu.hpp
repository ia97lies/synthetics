//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_CONSTRUCTOR_CONTEXT_MENU_H
#define SYNTHETICS_CONSTRUCTOR_CONTEXT_MENU_H

#include "Polycode.h"
#include "Polycode2DPhysics.h"
#include "Configurator.hpp"
#include "Controller.hpp"

namespace Synthetics {
  namespace Constructor {
    class ContextMenu : public Controller {
      public:
        ContextMenu(Polycode::Core *core, Configurator *conf);
        virtual ~ContextMenu();

        virtual void deactivate();
        virtual void activate();

        void handleEvent(Polycode::Event *e);

      private:
        bool m_on;
        typedef Controller super;
        Polycode::Core *m_core;
        Configurator *m_conf;
        Polycode::PhysicsScene2D *m_scene;
    };
  }
}

#endif