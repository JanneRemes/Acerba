#include <Ace/Material.h>
#include <Ace/GraphicsDevice.h>

#include <Ace/Platform.h>

#if ACE_LINUX
	#include <cstring> // strcpy
#endif

namespace ace
{
	Material::Material(MaterialImpl* impl) : Graphics(impl)
	{

	}

	Material::Material(const Shader& vertex, const Shader& fragment) : Graphics(nullptr)
	{
		Init(vertex, fragment);
	}

	bool Material::Init(const Shader& vertex, const Shader& fragment)
	{
		*this = GraphicsDevice::CreateMaterial(vertex, fragment);
		return (*this);
	}

	void Material::AddTexture(const Texture& texture, const char* name, UInt8 id)
	{
		textures[id] = texture;
		strcpy(textureNames[id], name);
	}

	void Material::Uniform(const char* name, const UInt32& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::UInt32, 1);
	}

	void Material::Uniform(const char* name, const Int32& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Int32, 1);
	}

	void Material::Uniform(const char* name, const float& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Float, 1);
	}
		 
	void Material::Uniform(const char* name, const Vector2& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec2, 1);
	}
	
	void Material::Uniform(const char* name, const Vector3& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec3, 1);
	}
	
	void Material::Uniform(const char* name, const Vector4& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec4, 1);
	}
	
	void Material::Uniform(const char* name, const Color32& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec4, 1);
	}
		 
	void Material::Uniform(const char* name, const Matrix2& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value.array, UniformType::Mat2, 1);
	}
	
	void Material::Uniform(const char* name, const Matrix3& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value.array, UniformType::Mat3, 1);
	}
	
	void Material::Uniform(const char* name, const Matrix4& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value.array, UniformType::Mat4, 1);
	}	

	void Material::Uniform(const char* name, const UInt32* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::UInt32, count);
	}
	
	void Material::Uniform(const char* name, const Int32* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Int32, count);
	}
	
	void Material::Uniform(const char* name, const float* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Float, count);
	}
		
	void Material::Uniform(const char* name, const Vector2* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Vec2, count);
	}
	
	void Material::Uniform(const char* name, const Vector3* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Vec3, count);
	}
	
	void Material::Uniform(const char* name, const Vector4* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Vec4, count);
	}
	
	void Material::Uniform(const char* name, const Color32* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Vec4, count);
	}
		
	void Material::Uniform(const char* name, const Matrix2* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Mat2, count);
	}
	
	void Material::Uniform(const char* name, const Matrix3* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Mat3, count);
	}
	
	void Material::Uniform(const char* name, const Matrix4* values, UInt32 count) const
	{
		GraphicsDevice::Uniform(*this, name, values, UniformType::Mat4, count);
	}
}