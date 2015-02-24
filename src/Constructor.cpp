//----------------------------------------------------------------------------
// The MIT License
//----------------------------------------------------------------------------

#include "PolyString.h"
#include "PolyUITreeEvent.h"

#include "Lua.hpp"
#include "PolycodeFacade.hpp"
#include "Part.hpp"
#include "OrbitCamera.hpp"
#include "Robot.hpp"
#include "FileManager.hpp"
#include "Constructor.hpp"

using namespace Polycode;

namespace Synthetics {
  Constructor::Constructor(Core *core, Configurator *conf, Components::Factory *factory) : EventHandler() {
    m_core = core;
    m_conf = conf;
    m_factory = factory;
    m_scene = new CollisionScene();
    m_camera = new OrbitCamera(m_core, m_scene);
    m_camera->activate(true);

    m_on = false;
    activate(true);

    m_mother = new Robot(new PolycodeFacade(m_core, m_scene));

    m_camera->update();

    m_selectorDisplay = new SelectorDisplay(m_core, m_conf, m_factory);

    
    Scene *scene = new Scene(Scene::SCENE_2D_TOPLEFT);
    scene->doVisibilityChecking(false);
    scene->getDefaultCamera()->frustumCulling = false;
    scene->rootEntity.processInputEvents = true;

    std::vector<String> extensions;
    extensions.push_back("lua");
    m_fileDialog = new FileManager(String("/home/cli"), extensions);
    scene->addEntity(m_fileDialog);
    m_fileDialog->hideWindow();

  }

  Constructor::~Constructor() {
  }

  void Constructor::handleEvent(Event *e) {
    if(e->getDispatcher() == m_core->getInput()) {
      InputEvent *inputEvent = (InputEvent*)e;

      switch(e->getEventCode()) {
        case InputEvent::EVENT_KEYDOWN:
          switch (inputEvent->keyCode()) {
            case KEY_DELETE:
              // delete active component
              if (m_mother) {
                m_mother->remove();
              }
              break;
            case KEY_RETURN:
              // add current selected component to current active component
              if (m_mother->isEmpty()) {
                Component *component = m_factory->createComponent(m_selectorDisplay->getText(), m_core, m_scene);
                m_mother->add(component);
              }
              else if (!m_mother->inPlace()) {
                Component *component = m_factory->createComponent(m_selectorDisplay->getText(), m_core, m_scene);
                m_mother->place(component);
              }
              else {
                m_mother->add();
              }
              break;
            case KEY_UP:
              {
                m_mother->rotateInPlace(1);
              }
              break;
            case KEY_DOWN:
              {
                m_mother->rotateInPlace(-1);
              }
              break;
            case KEY_o:
              {
                m_mother->powerOn(!m_mother->isPowerOn());
              }
              break;
            case KEY_s:
              {
                m_fileDialog->save(m_mother, NULL);
              }
              break;
            case KEY_l:
              {
                delete m_mother;
                m_mother = new Robot(new PolycodeFacade(m_core, m_scene));

                Lua *lua = new Lua();
                lua->open();
                lua->setCPath("./lib/?.so");
                lua_State *L = lua->L();
                lua_pushlightuserdata(L, m_core);
                lua_setfield(L, LUA_REGISTRYINDEX, "core");
                lua_pushlightuserdata(L, m_scene);
                lua_setfield(L, LUA_REGISTRYINDEX, "scene");
                lua_pushlightuserdata(L, m_factory);
                lua_setfield(L, LUA_REGISTRYINDEX, "factory");
                lua_pushlightuserdata(L, m_mother);
                lua_setfield(L, LUA_REGISTRYINDEX, "robot");

                if (luaL_loadfile(L, ".snapshot.lua") || lua_pcall(L, 0, 0, 0)) {
                  lua->error("cannot load .snapshot.lua: %s\n", lua_tostring(L, -1));
                }
                delete lua;
              }
          }
          break;
        case InputEvent::EVENT_MOUSEDOWN:
          switch(inputEvent->getMouseButton()) {
            case CoreInput::MOUSE_BUTTON1:
              Ray ray = m_scene->projectRayFromCameraAndViewportCoordinate(m_scene->getActiveCamera(), inputEvent->mousePosition);
              RayTestResult res = m_scene->getFirstEntityInRay(ray.origin, ray.direction * 300.0);
              if(res.entity) {
                m_mother->activate(res.entity);
              }
              if (m_mother->getActiveKnob()) {
                m_camera->activate(false);
              }
              break;
          }
          break;
        case InputEvent::EVENT_MOUSEUP:
          switch(inputEvent->getMouseButton()) {
            case CoreInput::MOUSE_BUTTON1:
              m_mother->deactivate();
              m_camera->activate(true);
              break;
          }
          break;
        case InputEvent::EVENT_MOUSEMOVE:
          // TODO only if mouse left mouse button down
          Vector3 delta(m_core->getInput()->getMouseDelta().x, m_core->getInput()->getMouseDelta().y, 0);
          m_mother->mouseMove(delta);
          break;
      }
    }
    else if (e->getEventType() == "UIEvent") {
      fprintf(stderr, "XXX: UIEvent\n");
      switch(e->getEventCode()) {
        case  UIEvent::OK_EVENT:
        case  UIEvent::CANCEL_EVENT:
          {
            fprintf(stderr, "XXX: cancel/ok\n");
          }
          break;
      }
    }
    else {
      fprintf(stderr, "XXX: UnknownEvent\n");
    }
  }

  void Constructor::update() {
    /*
    */
    m_fileDialog->Update();
    m_mother->update();
  }

  void Constructor::activate(bool on) {
    if (on && !m_on) {
      m_core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
      m_core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
      m_core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
      m_core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
      //m_core->getInput()->removeEventListener(this, UIEvent::OK_EVENT);
      //m_core->getInput()->removeEventListener(this, UIEvent::CANCEL_EVENT);
      m_core->getInput()->addEventListener(this, UIEvent::OK_EVENT);
      m_core->getInput()->addEventListener(this, UIEvent::CANCEL_EVENT);
    }
    else if (!on && m_on) {
      m_core->getInput()->removeEventListener(this, InputEvent::EVENT_KEYDOWN);
      m_core->getInput()->removeEventListener(this, InputEvent::EVENT_MOUSEDOWN);
      m_core->getInput()->removeEventListener(this, InputEvent::EVENT_MOUSEUP);
      m_core->getInput()->removeEventListener(this, InputEvent::EVENT_MOUSEMOVE);
      m_core->getInput()->addEventListener(this, UIEvent::OK_EVENT);
      m_core->getInput()->addEventListener(this, UIEvent::CANCEL_EVENT);
    }
    m_on = on;
  }

}

