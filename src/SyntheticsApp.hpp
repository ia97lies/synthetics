//----------------------------------------------------------------------------
// The MIT License
// Took the basics from Polycode Examples
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_APP_H
#define SYNTHETICS_APP_H

#include <Polycode.h>
#include "PolycodeView.h"
#include "bullet/LinearMath/btVector3.h"
#include "bullet/LinearMath/btMatrix3x3.h"
#include "bullet/LinearMath/btQuaternion.h"

using namespace Polycode;

namespace synthetics {

#define RADIANS_PER_DEGREE 0.01745329f
#define CAMERA_STEP_SIZE 5.0f

  class SyntheticsApp : public EventHandler {
    public:
      SyntheticsApp(PolycodeView *view);
      ~SyntheticsApp();
      void handleEvent(Event *e);
      bool Update();

    private:
      Core *m_core;
      Scene *m_scene;
      ScenePrimitive *m_curBox;

      void addChildBox(Vector3 orientation); 
  };
}
#endif
