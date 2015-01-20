//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_COMPONENTS_ACTOR_LIGHT_H
#define SYNTHETICS_COMPONENTS_ACTOR_LIGHT_H

#include "Part.hpp"
#include "Component.hpp"

namespace Synthetics {
  namespace Components {
    namespace Active {

      class Light : public Component {
        public:
          Light (Polycode::Scene *scene);
          virtual ~Light();

          virtual int getNoParts();
          virtual Part *getPart(int i);
          virtual void enable(bool on); 

        private:
          Polycode::Scene *m_scene;
          Part *m_body;
          Knob *m_knob;
          Polycode::SceneLight *m_light;
      };

    }
  }
}

#endif