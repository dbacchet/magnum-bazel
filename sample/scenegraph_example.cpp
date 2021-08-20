#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Grid.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "ImGuizmo.h"

#include <iostream>

namespace Magnum {
namespace Examples {

using namespace Magnum::Math::Literals;

using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;

class FlatDrawable : public SceneGraph::Drawable3D {
  public:
    explicit FlatDrawable(Object3D &object, Shaders::Flat3D &shader, GL::Mesh &mesh,
                          SceneGraph::DrawableGroup3D &drawables)
    : SceneGraph::Drawable3D{object, &drawables}
    , _shader(shader)
    , _mesh(mesh) {}

    void draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) {
        _shader.setColor(0x747474_rgbf).setTransformationProjectionMatrix(camera.projectionMatrix() * transformation);
        _mesh.draw(_shader);
    }

  private:
    Shaders::Flat3D &_shader;
    GL::Mesh &_mesh;
};

class PhongDrawable : public SceneGraph::Drawable3D {
  public:
    explicit PhongDrawable(Object3D &object, Shaders::Phong &shader, GL::Mesh &mesh,
                           SceneGraph::DrawableGroup3D &drawables)
    : SceneGraph::Drawable3D{object, &drawables}
    , _shader(shader)
    , _mesh(mesh) {}

    void draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) {
        _shader.setDiffuseColor(_color)
            .setLightPosition(camera.cameraMatrix().transformPoint({5.0f, 5.0f, 7.0f}))
            .setTransformationMatrix(transformation)
            .setNormalMatrix(transformation.normalMatrix())
            .setProjectionMatrix(camera.projectionMatrix());
        _mesh.draw(_shader);
    }

  private:
    Shaders::Phong &_shader;
    GL::Mesh &_mesh;
    Color3 _color = 0xa5c9ea_rgbf;
};

class ScenegraphExample : public Platform::Application {
  public:
    explicit ScenegraphExample(const Arguments &arguments);

  private:
    void drawEvent() override;

    void viewportEvent(ViewportEvent &event) override;

    void keyPressEvent(KeyEvent &event) override;
    void keyReleaseEvent(KeyEvent &event) override;

    void mousePressEvent(MouseEvent &event) override;
    void mouseReleaseEvent(MouseEvent &event) override;
    void mouseMoveEvent(MouseMoveEvent &event) override;
    void mouseScrollEvent(MouseScrollEvent &event) override;
    void textInputEvent(TextInputEvent &event) override;

    GL::Mesh _mesh;
    GL::Mesh _grid;
    Shaders::Phong _phongShader;
    Shaders::Flat3D _flatShader{NoCreate};

    SceneGraph::DrawableGroup3D _drawables;
    Scene3D _scene;
    Object3D *_refcube_object;
    Object3D *_cameraObject_azimuth;
    Object3D *_cameraObject_elevation;
    SceneGraph::Camera3D *_camera;
    GL::Mesh _grid_mesh;
    GL::Mesh _refcube_mesh;
    GL::Mesh _sphere_mesh;

    Matrix4 _projection;
    Color3 _color;

    float _camera_azimuth = 0.0;
    float _camera_elevation = 0.1;
    float _camera_distance = 10.0;

    ImGuiIntegration::Context _imgui{NoCreate};
    Vector3 _refcube_pos{0, 0, 0};
    Matrix4 _refcube_tf;
};

ScenegraphExample::ScenegraphExample(const Arguments &arguments)
: Platform::Application{
      arguments, Configuration{}.setTitle("Magnum Example").setWindowFlags(Configuration::WindowFlag::Resizable)} {
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    _flatShader = Shaders::Flat3D{};

    _grid_mesh = MeshTools::compile(Primitives::grid3DWireframe({199, 199}));
    auto grid = new Object3D{&_scene};
    (*grid)
        .scale(Vector3{100.0f});
    new FlatDrawable{*grid, _flatShader, _grid_mesh, _drawables};

    _refcube_mesh = MeshTools::compile(Primitives::cubeSolid());
    _refcube_object = new Object3D{&_scene};
    // (*_refcube_object)
    //     .scale(Vector3{1.0f});
    new PhongDrawable{*_refcube_object, _phongShader, _refcube_mesh, _drawables};

    // small spheres at different positions
    _sphere_mesh = MeshTools::compile(Primitives::icosphereSolid(2));
    auto sphere_obj = new Object3D{&_scene};
    new PhongDrawable{*sphere_obj, _phongShader, _sphere_mesh, _drawables};
    sphere_obj->resetTransformation()
        .scale(Vector3{0.2f})
        .translate({0.0f,0.0f,0.0f});
    sphere_obj = new Object3D{&_scene};
    new PhongDrawable{*sphere_obj, _phongShader, _sphere_mesh, _drawables};
    sphere_obj->resetTransformation()
        .scale(Vector3{0.1f})
        .translate({1.0f,1.0f,0.0f});
    sphere_obj = new Object3D{&_scene};
    new PhongDrawable{*sphere_obj, _phongShader, _sphere_mesh, _drawables};
    sphere_obj->resetTransformation()
        .scale(Vector3{0.1f})
        .translate({-1.0f,1.0f,0.0f});
    sphere_obj = new Object3D{&_scene};
    new PhongDrawable{*sphere_obj, _phongShader, _sphere_mesh, _drawables};
    sphere_obj->resetTransformation()
        .scale(Vector3{0.1f})
        .translate({1.0f,-1.0f,0.0f});
    sphere_obj = new Object3D{&_scene};
    new PhongDrawable{*sphere_obj, _phongShader, _sphere_mesh, _drawables};
    sphere_obj->resetTransformation()
        .scale(Vector3{0.1f})
        .translate({-1.0f,-1.0f,0.0f});
    // Set up the camera
    _cameraObject_azimuth = new Object3D{&_scene};
    _cameraObject_elevation = new Object3D{_cameraObject_azimuth};
    _camera = new SceneGraph::Camera3D{*_cameraObject_elevation};
    _camera->setProjectionMatrix(
        Matrix4::perspectiveProjection(45.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 1000.0f));

    _mesh = MeshTools::compile(Primitives::cubeSolid());
    _grid = MeshTools::compile(Primitives::grid3DWireframe({15, 15}));

    _projection = Matrix4::perspectiveProjection(35.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f) *
                  Matrix4::translation(Vector3::zAxis(-10.0f));
    _color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});

    // imgui
    _imgui = ImGuiIntegration::Context(Vector2{windowSize()} / dpiScaling(), windowSize(), framebufferSize());

    /* Set up proper blending to be used by ImGui. There's a great chance
       you'll need this exact behavior for the rest of your scene. If not, set
       this only for the drawFrame() call. */
    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
                                   GL::Renderer::BlendFunction::OneMinusSourceAlpha);
}

void ScenegraphExample::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    // _refcube_object->rotateX(1.0_degf);
    // _refcube_object->resetTransformation().translate(_refcube_pos);

    (*_cameraObject_elevation)
        .resetTransformation()
        .translate(Vector3::zAxis(_camera_distance))
        .rotateX(Math::Rad<float>(_camera_elevation));
    (*_cameraObject_azimuth).resetTransformation().rotateZ(Math::Rad<float>(_camera_azimuth));

    _camera->draw(_drawables);

    // imgui
    _imgui.newFrame();
    ImGuizmo::BeginFrame();

    /* Enable text input, if needed */
    if (ImGui::GetIO().WantTextInput && !isTextInputActive())
        startTextInput();
    else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
        stopTextInput();

    static int manip_operation = (int)ImGuizmo::TRANSLATE;
    static int manip_mode = (int)ImGuizmo::LOCAL;
    static bool enable_manipulator = false;
    {
        ImGui::Begin("cube position");

        // ImGui::SliderFloat("pos x", &_refcube_pos.x(), -10.0, 10.0);
        // ImGui::SliderFloat("pos y", &_refcube_pos.y(), -10.0, 10.0);
        // ImGui::SliderFloat("pos z", &_refcube_pos.z(), -10.0, 10.0);
        ImGui::SliderFloat("pos x", &_refcube_tf[3][0], -10.0, 10.0);
        ImGui::SliderFloat("pos y", &_refcube_tf[3][1], -10.0, 10.0);
        ImGui::SliderFloat("pos z", &_refcube_tf[3][2], -10.0, 10.0);
        ImGui::Checkbox("Manipulator", &enable_manipulator);
        if (enable_manipulator) {
            ImGui::Text("operation");
            ImGui::RadioButton("translate", &manip_operation, (int)ImGuizmo::TRANSLATE); ImGui::SameLine();
            ImGui::RadioButton("rotate", &manip_operation, (int)ImGuizmo::ROTATE);    ImGui::SameLine();
            ImGui::RadioButton("scale", &manip_operation, (int)ImGuizmo::SCALE);
            ImGui::Text("mode");
            ImGui::RadioButton("local", &manip_mode, (int)ImGuizmo::LOCAL); ImGui::SameLine();
            ImGui::RadioButton("world", &manip_mode, (int)ImGuizmo::WORLD);
        }
        ImGui::End();
    }
    // /* 1. Show a simple window.
    //    Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appear
    //    in a window called "Debug" automatically */
    // {
    //     ImGui::Text("Hello, world!");
    //     ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);
    //     if(ImGui::ColorEdit3("Clear Color", _clearColor.data()))
    //         GL::Renderer::setClearColor(_clearColor);
    //     if(ImGui::Button("Test Window"))
    //         _showTestWindow ^= true;
    //     if(ImGui::Button("Another Window"))
    //         _showAnotherWindow ^= true;
    //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
    //         1000.0/Double(ImGui::GetIO().Framerate),
    //         Double(ImGui::GetIO().Framerate));
    // }

    // /* 2. Show another simple window, now using an explicit Begin/End pair */
    // if(_showAnotherWindow) {
    //     ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    //     ImGui::Begin("Another Window", &_showAnotherWindow);
    //     ImGui::Text("Hello");
    //     ImGui::End();
    // }

    /* 3. Show the ImGui test window. Most of the sample code is in
       ImGui::ShowDemoWindow() */
    if (true) {
        ImGui::SetNextWindowPos(ImVec2(150, 20)); //, ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();
    }

    // object manipulator
    if (enable_manipulator) {
        ImGuiIO &io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        ImGuizmo::Manipulate(_camera->cameraMatrix().data(), _camera->projectionMatrix().data(), 
                (ImGuizmo::OPERATION)manip_operation, (ImGuizmo::MODE)manip_mode, _refcube_tf.data(), 
                NULL, NULL, NULL, NULL);
    }
    _refcube_object->setTransformation(_refcube_tf);

    /* Set appropriate states. If you only draw ImGui, it is sufficient to
       just enable blending and scissor test in the constructor. */
    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    _imgui.drawFrame();

    /* Reset state. Only needed if you want to draw something else with
       different state after. */
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::Blending);
    swapBuffers();
    redraw();
}

// void ScenegraphExample::mouseMoveEvent(MouseMoveEvent& event) {
//     if(!(event.buttons() & MouseMoveEvent::Button::Left)) return;
//
//     Vector2 delta
//     = 3.0f*Vector2{event.relativePosition()}/Vector2{windowSize()};
//
//     _camera_azimuth -= delta.x();
//     _camera_elevation -= delta.y();
//
//     event.setAccepted();
//     redraw();
// }

// void ScenegraphExample::mouseScrollEvent(MouseScrollEvent& event) {
//     auto offset = event.offset();
//     std::cout << offset.x() << " " << offset.y() << std::endl;
//
//     _camera_distance *= 1.0f + offset.y()/10.0;
//     if (_camera_distance<0.1) _camera_distance = 0.1;
//     redraw();
// }

void ScenegraphExample::viewportEvent(ViewportEvent &event) {
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    _camera->setProjectionMatrix(
        Matrix4::perspectiveProjection(45.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f));

    _imgui.relayout(Vector2{event.windowSize()} / event.dpiScaling(), event.windowSize(), event.framebufferSize());
}

void ScenegraphExample::keyPressEvent(KeyEvent &event) {
    if (_imgui.handleKeyPressEvent(event))
        return;
}

void ScenegraphExample::keyReleaseEvent(KeyEvent &event) {
    if (_imgui.handleKeyReleaseEvent(event))
        return;
}

void ScenegraphExample::mousePressEvent(MouseEvent &event) {
    if (_imgui.handleMousePressEvent(event))
        return;
}

void ScenegraphExample::mouseReleaseEvent(MouseEvent &event) {
    if (_imgui.handleMouseReleaseEvent(event))
        return;
}

void ScenegraphExample::mouseMoveEvent(MouseMoveEvent &event) {
    if (_imgui.handleMouseMoveEvent(event))
        return;

    if (!(event.buttons() & MouseMoveEvent::Button::Left))
        return;

    Vector2 delta = 3.0f * Vector2{event.relativePosition()} / Vector2{windowSize()};

    _camera_azimuth -= delta.x();
    _camera_elevation -= delta.y();

    event.setAccepted();
    redraw();
}

void ScenegraphExample::mouseScrollEvent(MouseScrollEvent &event) {
    if (_imgui.handleMouseScrollEvent(event)) {
        /* Prevent scrolling the page */
        event.setAccepted();
        // redraw();
        return;
    }
    auto offset = event.offset();

    _camera_distance *= 1.0f + offset.y() / 10.0;
    if (_camera_distance < 0.1)
        _camera_distance = 0.1;
    redraw();
}

void ScenegraphExample::textInputEvent(TextInputEvent &event) {
    if (_imgui.handleTextInputEvent(event))
        return;
}

} // namespace Examples
} // namespace Magnum

MAGNUM_APPLICATION_MAIN(Magnum::Examples::ScenegraphExample)
