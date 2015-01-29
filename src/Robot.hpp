//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_ROBOT_H
#define SYNTHETICS_ROBOT_H

#include "Polycode.h"
#include "Polycode3DPhysics.h"
#include "PolycodeFacade.hpp"
#include "Compound.hpp"
#include "Component.hpp"

namespace Synthetics {
  class Robot: public Compound {
    public:
      Robot(PolycodeFacade *facade); 
      virtual ~Robot();

      void save(std::string file);
      void load(std::string file);

      bool isEmpty(); 
      bool inPlace(); 
      void place(Component *component);
      void add(Component *component);
      void add();
      void remove();
      void activate(Polycode::Entity *plugShape); 
      void deactivate(); 
      void rotateInPlace(int direction);
      void mouseMove(Polycode::Vector3 delta);
      Plug *getActivePlug();
      Knob *getActiveKnob();

      void update();

      static void constructGraphic(PolycodeFacade *facade, Part *parent, Component *component);
      static void constructPlugsGraphic(PolycodeFacade *facade, Part *curPart); 
      static void constructKnobGraphic(PolycodeFacade *facade, Part *curPart); 
      static void destructGraphic(PolycodeFacade *facade, Component *component); 

    private:
      long m_curId;
      PolycodeFacade *m_polycodeFacade;
      Component *m_mother;
      Component *m_inPlace;

      Component *m_activeComponent;
      Plug *m_activePlug;
      Part *m_activePart;
      Knob *m_activeKnob;

      Plug *m_inPlacePlug;
      Part *m_inPlacePart;
  };
}

#endif
