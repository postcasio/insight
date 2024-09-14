#include "OrbitCameraController.h"

#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Editors/MaterialEditor/Nodes/AddNode.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/TransformComponent.h"

namespace Portent
{
    void OrbitCameraController::Update(float delta)
    {
        if (!m_Camera.IsValid() || !m_FocusedEntity.IsValid())
        {
            return;
        }

        auto& cameraTransformComponent = m_Camera.GetComponent<TransformComponent>();
        auto& cameraComponent = m_Camera.GetComponent<CameraComponent>();
        auto& focusedEntityTransformComponent = m_FocusedEntity.GetComponent<TransformComponent>();

        cameraTransformComponent.Position = GetCameraPosition();
        cameraTransformComponent.LookAt(focusedEntityTransformComponent.Position + m_Center);
    }

    void OrbitCameraController::RotateLeft(float radians)
    {
        m_AzimuthAngle = glm::mod(m_AzimuthAngle + radians, glm::two_pi<float>());

        if (m_AzimuthAngle < 0.0f)
        {
            m_AzimuthAngle += glm::two_pi<float>();
        }
    }

    void OrbitCameraController::RotateUp(float radians)
    {
        constexpr auto cap = glm::half_pi<float>() - glm::epsilon<float>();

        m_PolarAngle = glm::clamp(m_PolarAngle + radians, -cap, cap);
    }

    void OrbitCameraController::Zoom(float zoom)
    {
        m_Radius = glm::clamp(m_Radius - zoom, m_MinRadius, m_MaxRadius);
    }

    void OrbitCameraController::PanHorizontal(float pan)
    {
        const auto position = GetCameraPosition();
        const auto view = GetViewDirection();
        const auto strafe = glm::normalize(glm::cross(view, {0, 1, 0}));

        m_Center += strafe * pan;
    }

    void OrbitCameraController::PanVertical(float pan)
    {
        m_Center += vec3(0, pan, 0);
    }

    vec3 OrbitCameraController::GetViewDirection()
    {
        return glm::normalize(m_Center - GetCameraPosition());
    }

    vec3 OrbitCameraController::GetCameraPosition() const
    {
        const auto sineAzimuth = glm::sin(m_AzimuthAngle);
        const auto cosineAzimuth = glm::cos(m_AzimuthAngle);
        const auto sinePolar = glm::sin(m_PolarAngle);
        const auto cosinePolar = glm::cos(m_PolarAngle);

        const auto x = m_Center.x + m_Radius * cosinePolar * cosineAzimuth;
        const auto y = m_Center.y + m_Radius * sinePolar;
        const auto z = m_Center.z + m_Radius * cosinePolar * sineAzimuth;

        return {x, y, z};
    }
} // Portent
