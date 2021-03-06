#include <Ace/Matrix3.h>
#include <Ace/Vector2.h>
#include <Ace/Math.h>

namespace ace
{
    namespace math
    {

        Matrix3::Matrix3() : rows() {}

        Matrix3::Matrix3(const Vector3& v1, const Vector3& v2, const Vector3& v3)
        {
            rows[0] = v1;
            rows[1] = v2;
            rows[2] = v3;
        }

        Matrix3::Matrix3(float identity) : rows()
        {
            data[0][0] = data[1][1] = data[2][2] = identity;
        }

        float Matrix3::Determinant() const
        {
            const Matrix3& m = *this;
            return
                m(0, 0) * (m(1, 1)*m(2, 2) - m(1, 2)*m(2, 1)) -
                m(0, 1) * (m(1, 0)*m(2, 2) - m(1, 2)*m(2, 0)) +
                m(0, 2) * (m(1, 0)*m(2, 1) - m(1, 1)*m(2, 0));
        }

        Matrix3 Matrix3::Transpose() const
        {
            Matrix3 t;

            t(0, 0) = (*this)(0, 0);
            t(1, 1) = (*this)(1, 1);
            t(2, 2) = (*this)(2, 2);
            t(1, 2) = (*this)(2, 1);
            t(2, 1) = (*this)(1, 2);
            t(0, 1) = (*this)(1, 0);
            t(0, 2) = (*this)(2, 0);
            t(1, 0) = (*this)(0, 1);
            t(2, 0) = (*this)(0, 2);

            return t;

        }

        Matrix3 Matrix3::Inverse() const
        {
            return Adjunct() * (1.f / Determinant());
        }

        Matrix3 Matrix3::Cofactor() const
        {
            Matrix3 m;


            m(0, 0) = (data[1][1] * data[2][2] - data[1][2] * data[2][1]);
            m(0, 1) = -1 * ((data[1][0] * data[2][2] - data[1][2] * data[2][0]));
            m(0, 2) = (data[1][0] * data[2][1] - data[1][1] * data[2][0]);

            m(1, 0) = -1 * ((data[0][1] * data[2][2] - data[0][2] * data[2][1]));
            m(1, 1) = (data[0][0] * data[2][2] - data[0][2] * data[2][0]);
            m(1, 2) = -1 * ((data[0][0] * data[2][1] - data[0][1] * data[2][0]));

            m(2, 0) = (data[0][1] * data[1][2] - data[0][2] * data[1][1]);
            m(2, 1) = -1 * ((data[0][0] * data[1][2] - data[0][2] * data[1][0]));
            m(2, 2) = (data[0][0] * data[1][1] - data[0][1] * data[1][0]);

            return m;
        }

        Matrix3 Matrix3::Adjunct() const
        {
            return Cofactor().Transpose();
        }

        Matrix3 Matrix3::operator*(const Matrix3& m) const
        {
            return Matrix3(
                Vector3(rows[0].x*m(0, 0) + rows[0].y*m(1, 0) + rows[0].z*m(2, 0), rows[0].x*m(0, 1) + rows[0].y*m(1, 1) + rows[0].z*m(2, 1), rows[0].x*m(0, 2) + rows[0].y*m(1, 2) + rows[0].z*m(2, 2)),
                Vector3(rows[1].x*m(0, 0) + rows[1].y*m(1, 0) + rows[1].z*m(2, 0), rows[1].x*m(0, 1) + rows[1].y*m(1, 1) + rows[1].z*m(2, 1), rows[1].x*m(0, 2) + rows[1].y*m(1, 2) + rows[1].z*m(2, 2)),
                Vector3(rows[2].x*m(0, 0) + rows[2].y*m(1, 0) + rows[2].z*m(2, 0), rows[2].x*m(0, 1) + rows[2].y*m(1, 1) + rows[2].z*m(2, 1), rows[2].x*m(0, 2) + rows[2].y*m(1, 2) + rows[2].z*m(2, 2))
            );
        }

        Vector3 Matrix3::operator*(const Vector3& o) const
        {
            return Vector3(
                rows[0].x * o.x + rows[0].y * o.y + rows[0].z * o.z,
                rows[1].x * o.x + rows[1].y * o.y + rows[1].z * o.z,
                rows[2].x * o.x + rows[2].y * o.y + rows[2].z * o.z
            );
        }

        Matrix3 Matrix3::operator*(float s) const
        {
            Matrix3 mat = *this;

            for (int i = 0; i < 9; ++i)
            {
                mat.array[i] *= s;
            }

            return mat;
        }

        Matrix3 Matrix3::Identity()
        {
            return Matrix3(1);
        }


        Matrix3 Matrix3::Scale(float x, float y, float z)
        {
            return Matrix3(
                Vector3(x, 0.f, 0.f),
                Vector3(0.f, y, 0.f),
                Vector3(0.f, 0.f, z)
            );
        }

        Matrix3 Matrix3::Translation(const Vector2& t)
        {
            return Matrix3(
                Vector3(1.f, 0.f, 0.f),
                Vector3(0.f, 1.f, 0.f),
                Vector3(t.x, t.y, 1.f)
            );
        }

        Matrix3 Matrix3::RotationX(float a)
        {
            a = Rad(a);
            return Matrix3(
                Vector3(1.f, 0.f, 0.f),
                Vector3(0.f, Cos(a), -Sin(a)),
                Vector3(0.f, Sin(a), Cos(a))
            );
        }

        Matrix3 Matrix3::RotationZ(float a)
        {
            a = Rad(a);
            return Matrix3(
                Vector3(Cos(a), -Sin(a), 0.f),
                Vector3(Sin(a), Cos(a), 0.f),
                Vector3(0.f, 0.f, 1.f)
            );
        }

        Matrix3 Matrix3::RotationY(float a)
        {
            a = Rad(a);
            return Matrix3(
                Vector3(Cos(a), 0.f, Sin(a)),
                Vector3(0.f, 1.f, 0.f),
                Vector3(-Sin(a), 0.f, Cos(a))
            );
        }
    }
}