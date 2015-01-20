//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_PLUG_H
#define SYNTHETICS_PLUG_H

#include "PolyEntity.h"
#include "PolyVector3.h"
#include "Compound.hpp"

namespace Synthetics {
  class Plug {
    public:
      Plug(Polycode::Vector3 position, Polycode::Vector3 rotation);
      virtual ~Plug();

      virtual Polycode::Vector3 getPosition();
      virtual Polycode::Vector3 getRotation();
      virtual Polycode::Vector3 getFaceToFaceRotation(Plug *plug); 
      virtual Polycode::Entity *getShape();
      virtual void activate(bool on);
      virtual bool isActivate(); 
      virtual void setCompound(Compound *compound);
      virtual void unsetCompound();
      virtual Compound *getCompound();

    private:
      Polycode::Vector3 m_position;
      Polycode::Vector3 m_rotation;
      Polycode::Entity *m_shape;
      bool m_on;
      Compound *m_compound;
  };
}

#endif
