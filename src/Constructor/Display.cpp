//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#include "PolyString.h"
#include "PolyUITreeEvent.h"

#include "PolycodeFacade.hpp"
#include "Part.hpp"
#include "OrbitCamera.hpp"
#include "Robot.hpp"
#include "FileManager.hpp"
#include "Display.hpp"

using namespace Polycode;

namespace Synthetics {
  namespace Constructor {
    class ChangeInPlace : public ChangeSelection {
      public:
        ChangeInPlace(Core *core, PhysicsScene *scene, Components::Factory *factory, Robot *robot) {
          m_core = core;
          m_scene = scene;
          m_factory = factory;
          m_robot = robot;
        }
        virtual ~ChangeInPlace() {}
        virtual void onChange(std::string name) {
          if (m_robot->inPlace()) {
            Component *component = m_factory->createComponent(name, m_core, m_scene);
            m_robot->replace(component);
          }
        }
      private:
        Core *m_core;
        PhysicsScene *m_scene;
        Components::Factory *m_factory;
        Robot *m_robot;
    };

    class DoneCompletion : public FileManagerCompletion {
      public:
        DoneCompletion(Display *constructor, MovingCamera *camera) {
          m_constructor = constructor;
          m_camera = camera;
        }
        virtual ~DoneCompletion() {}
        virtual void done() {
          m_constructor->activate(true);
          m_camera->activate(true);
        }
      private:
        Display *m_constructor;
        MovingCamera *m_camera;
    };

    Display::Display(Core *core, Configurator *conf, Components::Factory *factory) : EventHandler() {
      m_core = core;
      m_conf = conf;
      m_factory = factory;
      m_scene = new PhysicsScene();
      m_camera = new OrbitCamera(m_core, m_scene);
      m_camera->activate(true);
      m_lastClickTime = 0;

      m_on = false;
      activate(true);

      m_root = new Robot(new PolycodeFacade(m_core, m_scene));

      m_camera->update();

      m_selectorDisplay = new SelectorDisplay(m_core, m_conf, m_factory, new ChangeInPlace(m_core, m_scene, m_factory, m_root));


      Scene *scene = new Scene(Scene::SCENE_2D_TOPLEFT);
      scene->doVisibilityChecking(false);
      scene->getDefaultCamera()->frustumCulling = false;
      scene->rootEntity.processInputEvents = true;

      std::vector<String> extensions;
      extensions.push_back("*.lua");
      m_fileDialog = new FileManager(m_core, m_scene, m_factory, extensions);
      scene->addEntity(m_fileDialog);
    }

    Display::~Display() {
    }

    void Display::handleEvent(Event *e) {
      if(e->getDispatcher() == m_core->getInput()) {
        InputEvent *inputEvent = (InputEvent*)e;

        switch(e->getEventCode()) {
          case InputEvent::EVENT_KEYDOWN:
            switch (inputEvent->keyCode()) {
              case KEY_DELETE:
                // delete active component
                if (m_root) {
                  m_root->remove();
                }
                break;
              case KEY_RETURN:
                {
                  place();
                }
                break;
              case KEY_UP:
                {
                  m_root->rotateInPlace(1);
                }
                break;
              case KEY_DOWN:
                {
                  m_root->rotateInPlace(-1);
                }
                break;
              case KEY_o:
                {
                  m_root->powerOn(!m_root->isPowerOn());
                }
                break;
              case KEY_s:
                {
                  DoneCompletion *completion = new DoneCompletion(this, m_camera);
                  activate(false);
                  m_camera->activate(false);
                  m_fileDialog->save(m_root, completion);
                }
                break;
              case KEY_l:
                {
                  delete m_root;
                  m_root = new Robot(new PolycodeFacade(m_core, m_scene));
                  DoneCompletion *completion = new DoneCompletion(this, m_camera);
                  activate(false);
                  m_camera->activate(false);
                  m_fileDialog->load(m_root, completion);
                }
            }
            break;
          case InputEvent::EVENT_MOUSEDOWN:
            switch(inputEvent->getMouseButton()) {
              case CoreInput::MOUSE_BUTTON1:
                {
                  Ray ray = m_scene->projectRayFromCameraAndViewportCoordinate(m_scene->getActiveCamera(), inputEvent->mousePosition);
                  RayTestResult res = m_scene->getFirstEntityInRay(ray.origin, ray.direction * 300.0);
                  if(res.entity) {
                    m_root->activate(res.entity);
                  }
                  if (m_root->getActiveKnob()) {
                    m_camera->activate(false);
                  }
                  // double click, somehow InputEvent::EVENT_DOUBLECLICK won't work, do it by my self
                  unsigned int timestamp = m_core->getTicks();
                  if (timestamp - m_lastClickTime < 200) {
                    place();
                  }
                  m_lastClickTime = m_core->getTicks();
                }
                break;
            }
            break;
          case InputEvent::EVENT_MOUSEUP:
            switch(inputEvent->getMouseButton()) {
              case CoreInput::MOUSE_BUTTON1:
                {
                  m_root->deactivate();
                  m_camera->activate(true);
                }
                break;
            }
            break;
          case InputEvent::EVENT_MOUSEMOVE:
            {
              Vector3 delta(m_core->getInput()->getMouseDelta().x, m_core->getInput()->getMouseDelta().y, 0);
              m_root->mouseMove(delta);
            }
            break;
            break;
        }
      }
    }

    void Display::update() {
      m_fileDialog->Update();
      m_root->update();
    }

    void Display::activate(bool on) {
      if (on && !m_on) {
        m_core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
        m_core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
        m_core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
        m_core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
        m_core->getInput()->addEventListener(this, InputEvent::EVENT_DOUBLECLICK);
        m_core->getInput()->addEventListener(this, UIEvent::OK_EVENT);
        m_core->getInput()->addEventListener(this, UIEvent::CANCEL_EVENT);
      }
      else if (!on && m_on) {
        m_core->getInput()->removeEventListener(this, InputEvent::EVENT_KEYDOWN);
        m_core->getInput()->removeEventListener(this, InputEvent::EVENT_MOUSEDOWN);
        m_core->getInput()->removeEventListener(this, InputEvent::EVENT_MOUSEUP);
        m_core->getInput()->removeEventListener(this, InputEvent::EVENT_MOUSEMOVE);
        m_core->getInput()->removeEventListener(this, InputEvent::EVENT_DOUBLECLICK);
        m_core->getInput()->addEventListener(this, UIEvent::OK_EVENT);
        m_core->getInput()->addEventListener(this, UIEvent::CANCEL_EVENT);
      }
      m_on = on;
    }

    void Display::place() {
      // add current selected component to current active component
      if (m_root->isEmpty()) {
        Component *component = m_factory->createComponent(m_selectorDisplay->getText(), m_core, m_scene);
        m_root->setRoot(component);
      }
      else if (!m_root->inPlace()) {
        Component *component = m_factory->createComponent(m_selectorDisplay->getText(), m_core, m_scene);
        m_root->place(component);
      }
      else {
        m_root->add();
      }
    }
  }
}

