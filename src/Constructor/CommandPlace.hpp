//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#ifndef SYNTHETICS_CONSTRUCTOR_COMMAND_PLACE_H
#define SYNTHETICS_CONSTRUCTOR_COMMAND_PLACE_H

#include "Command.hpp"
#include "Robot.hpp"

namespace Synthetics {
  namespace Constructor {

    class CommandPlace : public Command {
      public:
        CommandPlace(Robot *robot, Component *component); 
        virtual ~CommandPlace();
        virtual void execute();
        virtual void undo();

      private:
        Robot *m_robot;
        Component *m_component;
        Plug *m_previousPlug;
    };
  }
}

#endif
