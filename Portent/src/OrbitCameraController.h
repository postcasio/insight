#pragma once

#include "Portent.h"
#include <Insight/Math.h>
#include <Insight/Entity.h>

namespace Portent {

    constexpr int SPHERICAL_RADIUS = 0;
    constexpr int SPHERICAL_PHI = 1;
    constexpr int SPHERICAL_THETA = 2;

    enum class OrbitCameraState
    {
        None
    };

class OrbitCameraController {
public:
    OrbitCameraController() = default;
    explicit OrbitCameraController(Insight::Entity camera, Insight::Entity target) : m_Camera(camera), m_FocusedEntity(target) {}
    ~OrbitCameraController() = default;

    void SetCamera(Insight::Entity camera) { m_Camera = camera; }
    void SetFocusedEntity(Insight::Entity target) { m_FocusedEntity = target; }

    void Update(float delta);
    void RotateLeft(float angle);
    void RotateUp(float radians);
    void Zoom(float zoom);
    void PanHorizontal(float pan);
    void PanVertical(float pan);

    [[nodiscard]] vec3 GetViewDirection();
    [[nodiscard]] vec3 GetCameraPosition() const;
private:

    Insight::Entity m_Camera;
    Insight::Entity m_FocusedEntity;

    vec3 m_Center = vec3(0.0f);
    float m_Radius = 14.0f;
    float m_MinRadius = 0.1f;
    float m_MaxRadius = 100.0f;
    float m_AzimuthAngle = 0.0f;
    float m_PolarAngle = 0.0f;


};

}
