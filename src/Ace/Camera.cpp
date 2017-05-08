#include <Ace/Camera.h>

namespace ace
{

    Camera::Camera(EntityManager& manager) :
        m_camera(manager)
    {
        //Move(-10);
    }

    const Entity& Camera::GetCamera() const
    {
        return m_camera;
    }


    Entity& Camera::GetCamera()
    {
        return m_camera;
    }


    Matrix4 Camera::GetVPMatrix() const
    {
        return m_proj * m_view;
    }


    void Camera::LookAt(const Vector3& target, const Vector3& up)
    {
        m_view = Matrix4::LookAt(m_camera->transform.position, target, up);
    }


    void Camera::MakeOrtho(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float znear,
        const float zfar
        )
    {
        m_proj = Matrix4::Ortho(left, right, bottom, top, znear, zfar);
    }


    void Camera::Move(const float z)
    {
        m_camera->transform.position.z += z;
    }


    void Camera::Move(const Vector3& v)
    {
        m_camera->transform.position += v;
    }


    void Camera::SetParent(Entity& parent)
    {
        parent.AddChild(m_camera);
    }


    void Camera::SetPosition(const Vector3& v)
    {
        m_camera->transform.position = v;
    }

}