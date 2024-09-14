#pragma once

#include "../Math.h"

namespace Insight
{
    struct TransformComponent
    {
        vec3 Position = {0.0f, 0.0f, 0.0f};
        quat Quaternion = {1.0f, 0.0f, 0.0f, 0.0f};
        vec3 Scale = {1.0f, 1.0f, 1.0f};
        //
        vec3 Rotation = {0.0f, 0.0f, 0.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        explicit TransformComponent(const glm::vec3 &position) : Position(position) {}

        [[nodiscard]] mat4 GetTransform() const
        {
            return glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(Quaternion) * glm::scale(glm::mat4(1.0f), Scale);

        }

        void LookAt(const vec3 target)
        {
            const mat4 lookAtMatrix = glm::transpose(glm::lookAt(Position, target, {0.0, 1.0, 0.0})); // position is the object's positio

            // Extract the rotation matrix part of the view matrix
            const auto rotationMatrix = mat3(lookAtMatrix);

            // Convert the rotation matrix to a quaternion
            Quaternion = glm::quat(rotationMatrix);
        }

        void TranslateOnAxis(const glm::vec3 axis, const float distance)
        {
            Position += this->Quaternion * glm::normalize(axis) * distance;
        }
        void TranslateX(const float distance)
        {
            TranslateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), distance);
        }
        void TranslateY(const float distance)
        {
            TranslateOnAxis(glm::vec3(0.0f, 1.0f, 0.0f), distance);
        }
        void TranslateZ(const float distance)
        {
            TranslateOnAxis(glm::vec3(0.0f, 0.0f, 1.0f), distance);
        }
        void RotateOnAxis(const glm::vec3 axis, float angle)
        {
            Quaternion = glm::angleAxis(angle, glm::normalize(axis)) * Quaternion;
        }
        void RotateX(const float angle)
        {
            RotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), angle);
        }
        void RotateY(const float angle)
        {
            RotateOnAxis(glm::vec3(0.0f, 1.0f, 0.0f), angle);
        }
        void RotateZ(const float angle)
        {
            RotateOnAxis(glm::vec3(0.0f, 0.0f, 1.0f), angle);
        }
    };
}
