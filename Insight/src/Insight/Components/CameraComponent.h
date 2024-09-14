#pragma once
#include "../Math.h"

namespace Insight
{
    enum class ProjectionType
    {
        Perspective = 0,
        Orthographic = 1
    };

    struct CameraComponent
    {
        mat4 Projection = mat4(1.0f);
        float NearPlane = 0.1f;
        float FarPlane = 32768;
        float FieldOfView = 45.0f;
        float Zoom = 1.0f;
        ProjectionType ProjectionType = ProjectionType::Perspective;

        CameraComponent() = default;
        CameraComponent(const CameraComponent &) = default;

        explicit CameraComponent(const glm::mat4 &projection) : Projection(projection)
        {
        }

        void SetPerspectiveProjection(const float fov, const float aspectRatio, const float near, const float far)
        {
            Projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
            Projection[1][1] *= -1;

            NearPlane = near;
            FarPlane = far;
            FieldOfView = fov;
            ProjectionType = ProjectionType::Perspective;
        }

        void SetOrthographicProjection(const float left, const float right, const float bottom, const float top, const float near, const float far)
        {
            Projection = glm::ortho(left, right, bottom, top, near, far);
            Projection[1][1] *= -1;

            NearPlane = near;
            FarPlane = far;
            FieldOfView = 0.0f;
            ProjectionType = ProjectionType::Orthographic;
        }

        void UpdateProjectionMatrix()
        {
            if (ProjectionType == ProjectionType::Perspective)
            {
                Projection = glm::perspective(glm::radians(FieldOfView), 1.0f, NearPlane, FarPlane);
                Projection[1][1] *= -1;
            }
            else if (ProjectionType == ProjectionType::Orthographic)
            {
                Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, NearPlane, FarPlane);
                Projection[1][1] *= -1;
            }
        }
    };
}
