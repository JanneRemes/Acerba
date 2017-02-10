#include <Ace/Quaternion.h>

namespace ace
{
	namespace math
	{

		Quaternion Quaternion::operator*(const Quaternion& q1) const
		{
			return{ vector.Cross(q1.vector) + q1.vector*scalar + vector*q1.scalar, scalar*q1.scalar - vector.Dot(q1.vector)};
		}

		Quaternion& Quaternion::operator*=(const Quaternion& q1)
		{
			vector = vector.Cross(q1.vector) + q1.vector*scalar + vector*q1.scalar;
			scalar = scalar*q1.scalar - vector.Dot(q1.vector);

			return *this;
		}

		Quaternion Quaternion::operator+(const Quaternion& q1) const
		{
			return{ vector + q1.vector, scalar + q1.scalar };
		}

		Quaternion& Quaternion::operator+=(const Quaternion& q1)
		{
			vector += q1.vector;
			scalar += q1.scalar;

			return *this;
		}

		Quaternion Quaternion::operator-(const Quaternion& q1) const
		{
			return{ vector - q1.vector, scalar - q1.scalar };
		}

		Quaternion& Quaternion::operator-=(const Quaternion& q1)
		{
			vector -= q1.vector;
			scalar -= q1.scalar;

			return *this;
		}


		Matrix2 Quaternion::ToMatrix2() const
		{
			return Matrix2(
				Vector2(2 * (Pow(scalar, 2) + Pow(vector.x, 2)) - 1, 2 * (vector.x*vector.y - scalar*vector.z)),
				Vector2(2 * (vector.x*vector.y + scalar*vector.z), 2 * (Pow(scalar, 2) + Pow(vector.y, 2)) - 1)
				);
		}

		Matrix3 Quaternion::ToMatrix3() const
		{
			return Matrix3(
				Vector3(2 * (Pow(scalar, 2) + Pow(vector.x, 2)) - 1, 2 * (vector.x*vector.y - scalar*vector.z), 2 * (vector.x*vector.z + scalar*vector.y)),
				Vector3(2 * (vector.x*vector.y + scalar*vector.z), 2 * (Pow(scalar, 2) + Pow(vector.y, 2)) - 1, 2 * (vector.y*vector.z - scalar*vector.x)),
				Vector3(2 * (vector.x*vector.z - scalar*vector.y), 2 * (vector.y*vector.z + scalar*vector.x), 2 * (Pow(scalar, 2) + Pow(vector.z, 2)) - 1)
				);
		}

		Matrix4 Quaternion::ToMatrix4() const
		{
			return Matrix4(
				Vector4(2 * (Pow(scalar, 2) + Pow(vector.x, 2)) - 1, 2 * (vector.x*vector.y - scalar*vector.z), 2 * (vector.x*vector.z + scalar*vector.y), 0),
				Vector4(2 * (vector.x*vector.y + scalar*vector.z), 2 * (Pow(scalar, 2) + Pow(vector.y, 2)) - 1, 2 * (vector.y*vector.z - scalar*vector.x), 0),
				Vector4(2 * (vector.x*vector.z - scalar*vector.y), 2 * (vector.y*vector.z + scalar*vector.x), 2 * (Pow(scalar, 2) + Pow(vector.z, 2)) - 1, 0),
				Vector4(0, 0, 0, 1)
				);
		}
	}

}