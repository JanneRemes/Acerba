#include <Ace/GraphicsDevice.h>
#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/Log.h>

#include <Ace/GL.h>
#include <Ace/GLBufferImpl.h>
#include <Ace/GLShaderImpl.h>
#include <Ace/GLMaterialImpl.h>
#include <Ace/GLTextureImpl.h>

namespace ace
{
	static const UInt32 GLBufferTargets[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
	static const UInt32 GLBufferUsage[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
	static const UInt32 GLShaderTypes[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

	static const UInt32 GLEnables[] = {
		0,
		GL_BLEND,
		GL_DEPTH_TEST,
		GL_STENCIL_TEST,
		GL_SCISSOR_TEST,
	};

	template <typename Type>
	inline UInt32 GetIndex(Type clear, UInt32 offset)
	{
		Int32 index = static_cast<Int32>(clear)-offset;
		return index > 0 ? index : 0;
	}

	inline void GLEnable(bool status, UInt32 index)
	{
		if (index == 0)
		{
			return;
		}

		if (status)
		{
			glEnable(GLEnables[index]);
		}
		else
		{
			glDisable(GLEnables[index]);
		}
	}

	void GraphicsDevice::Enable(bool status, Features features)
	{
		GLEnable(status, GetIndex(features & Features::Blend, 0));
		GLEnable(status, GetIndex(features & Features::Depth, 0));
		GLEnable(status, GetIndex(features & Features::Stencil, 1));
		GLEnable(status, GetIndex(features & Features::Scissor, 4));
	}


	void GraphicsDevice::Clear(const Color32& color, ClearFlags clear)
	{ 
		static const UInt32 GLClears[] = {
			0,
			GL_COLOR_BUFFER_BIT,
			GL_DEPTH_BUFFER_BIT,
			GL_STENCIL_BUFFER_BIT,
		};
				
		glClearColor(color.r, color.g, color.b, color.a);
		const UInt32 flags = (0x4 & 0x4) - 1;
		glClear(GLClears[GetIndex(clear & ClearFlags::Color, 0)] | GLClears[GetIndex(clear & ClearFlags::Depth, 0)] | GLClears[GetIndex(clear & ClearFlags::Stencil, 1)]);
	}

	void GraphicsDevice::Present(Window& window)
	{
		SDL_GL_SwapWindow((*window)->sdlWindow);

		// check OpenGL error
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::string error;

			switch (err) {
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
			}

			Logger::Log(Logger::Priority::Warning, "%s", error.c_str());
		}
	}

	void GraphicsDevice::Viewport(UInt32 w, UInt32 h)
	{
		glViewport(0, 0, w, h);
	}

	void GraphicsDevice::Scissor(Int32 x, Int32 y, UInt32 width, UInt32 height)
	{
		glScissor(x, y, width, height);
	}

	Buffer GraphicsDevice::CreateBuffer(BufferType type)
	{
		Buffer buffer;
		buffer.impl.reset(new Buffer::BufferImpl());
		buffer.type = type;
		return buffer;
	}

	void GraphicsDevice::BufferData(Buffer& buffer, UInt32 count, const Vertex* data, BufferUsage usage, UInt32 instances)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];
		buffer.size = count * instances;

		glBindBuffer(target, buffer.impl->bufferID);

		if (instances > 0)
		{
			Vertex* instance = new Vertex[count * instances];

			for (int j = 0; j < instances; ++j)
			{
				for (int i = 0; i < count; ++i)
				{
					instance[i + j * count] = data[i];
					instance[i + j * count].position.w = j;
				}
			}

			glBufferData(target, count * sizeof(Vertex) * instances, instance, GLBufferUsage[static_cast<UInt32>(usage)]);
			delete[] instance;
		}
		else
		{
			glBufferData(target, count * sizeof(Vertex), data, GLBufferUsage[static_cast<UInt32>(usage)]);

		}

		glBindBuffer(target, 0);

	}

	void GraphicsDevice::BufferSubData(Buffer& buffer, UInt32 count, UInt32 offset, const Vertex* data)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];

		glBindBuffer(target, buffer.impl->bufferID);
		glBufferSubData(target, offset, count * sizeof(Vertex), data);
		glBindBuffer(target, 0);
	}

	void GraphicsDevice::SetBuffer(const Buffer& buffer, BufferType type)
	{
		if (buffer.impl == nullptr)
		{
			return;
		}

		UInt32 target = GLBufferTargets[static_cast<UInt32>(type)];
		glBindBuffer(target, buffer.impl->bufferID);

		if (type == BufferType::Vertex)
		{
			glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)vertexAttributeSizes[0]);
			glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(vertexAttributeSizes[0] + vertexAttributeSizes[1]));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}
	}

	void GraphicsDevice::SetBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, buffer.type);
	}

	void GraphicsDevice::SetVertexBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, BufferType::Vertex);
	}

	void GraphicsDevice::SetIndexBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, BufferType::Index);
	}

	Texture GraphicsDevice::CreateTexture()
	{
		Texture texture;
		texture.impl.reset(new Texture::TextureImpl());

		return texture;
	}

	void GraphicsDevice::UpdateTexture(Texture& texture, const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format)
	{
		glBindTexture(GL_TEXTURE_2D, texture.impl->textureID);

		// TODO: Format & Filters

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		SetTextureFlags(texture);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GraphicsDevice::SetTexture(Material& material, const Texture& texture)
	{
		const UInt32 ID = 0;
		glUseProgram(material.impl->materialID);
		glActiveTexture(GL_TEXTURE0 + ID);
		glBindTexture(GL_TEXTURE_2D, texture.impl->textureID);
	}

	Shader GraphicsDevice::CreateShader(const char* source, ShaderType type)
	{
		Shader shader;

		shader.impl.reset(new Shader::ShaderImpl(GLShaderTypes[static_cast<UInt32>(type)]));
		shader.type = type;
		
		glShaderSource(shader.impl->shaderID, 1, &source, NULL);
		glCompileShader(shader.impl->shaderID);
		
		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;
		
		glGetShaderiv(shader.impl->shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader.impl->shaderID, GL_INFO_LOG_LENGTH, &errorMsgLength);
		
		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetShaderInfoLog(shader.impl->shaderID, errorMsgLength, NULL, errorMsg);
			Logger::Log(Logger::Priority::Error, "%s", errorMsg);
		
			delete[] errorMsg;
			shader.impl = nullptr;
		}

		return shader;
	}

	Material GraphicsDevice::CreateMaterial(const Shader& vertex, const Shader& fragment)
	{
		Material material;
		material.impl.reset(new Material::MaterialImpl());

		if (vertex.impl)
		{
			glAttachShader(material.impl->materialID, vertex.impl->shaderID);
		}

		if (fragment.impl)
		{
			glAttachShader(material.impl->materialID, fragment.impl->shaderID);
		}

		for (int i = 0; i < (int)VertexAttributes::COUNT; ++i)
		{
			glBindAttribLocation(material.impl->materialID, i, vertexAttributeNames[i]);
		}

		glLinkProgram(material.impl->materialID);
	
		if (vertex.impl)
		{
			glDetachShader(material.impl->materialID, vertex.impl->shaderID);
		}
		
		if (fragment.impl)
		{
			glDetachShader(material.impl->materialID, fragment.impl->shaderID);
		}
		
		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;

		glGetProgramiv(material.impl->materialID, GL_LINK_STATUS, &result);
		glGetProgramiv(material.impl->materialID, GL_INFO_LOG_LENGTH, &errorMsgLength);

		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetProgramInfoLog(material.impl->materialID, errorMsgLength, NULL, errorMsg);

			Logger::Log(Logger::Priority::Warning, "%s", errorMsg);
			delete[] errorMsg;

			material.impl = nullptr;

			// TODO: Set default error material.
		}		
		return material;
	}

	void GraphicsDevice::Uniform(Material& material, const char* name, const void* data, UniformType uniform, UInt32 elements)
	{
		glUseProgram(material.impl->materialID);
		UInt32 location = glGetUniformLocation(material.impl->materialID, name);

		switch (uniform)
		{
		case ace::UniformType::Int32:
			glUniform1iv(location, elements, static_cast<const Int32*>(data));
			break;
		case ace::UniformType::UInt32:
			glUniform1uiv(location, elements, static_cast<const UInt32*>(data));
			break;
		case ace::UniformType::Float:
			glUniform1fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Vec2:
			glUniform2fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Vec3:
			glUniform3fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Vec4:
			glUniform4fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Mat2:
			glUniformMatrix2fv(location, elements, false, static_cast<const math::Matrix2*>(data)->array);
			break;
		case ace::UniformType::Mat3:
			glUniformMatrix3fv(location, elements, false, static_cast<const math::Matrix3*>(data)->array);
			break;
		case ace::UniformType::Mat4:
			glUniformMatrix4fv(location, elements, false, static_cast<const math::Matrix4*>(data)->array);
			break;
		}
		
		glUseProgram(0);

	}

	void GraphicsDevice::Draw(Material& material, UInt32 elements, UInt32 indicies)
	{
		glUseProgram(material.impl->materialID);

		SetMaterialFlags(material);

		if (indicies == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, elements);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, indicies, GL_UNSIGNED_INT, 0);
		}
	}

	void GraphicsDevice::Draw(Material& material, const Mesh& mesh)
	{
		Enable(true, Features::Blend | Features::Depth);

		SetBuffer(mesh.vertexBuffer);
		SetBuffer(mesh.indexBuffer);

		Draw(material, mesh.GetElements(), mesh.GetIndicies());
	}

	void GraphicsDevice::SetMaterialFlags(Material& material)
	{
		static const UInt32 GLBlendModes[] = {
			GL_ZERO, 
			GL_ONE,
			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,
		};

		static const UInt32 GLTestFlags[] = {
			GL_ALWAYS,
			GL_NEVER,
			GL_LESS,
			GL_EQUAL,
			GL_LEQUAL,
			GL_GREATER,
			GL_NOTEQUAL,
			GL_GEQUAL,
		};

		static const UInt32 GLCullingModes[] = {
			GL_BACK,
			GL_FRONT,
			GL_FRONT_AND_BACK
		};

		glBlendFunc(GLBlendModes[static_cast<UInt32>(material.flags.blendModesSrc)], GLBlendModes[static_cast<UInt32>(material.flags.blendModesDst)]);
		glDepthFunc(GLTestFlags[static_cast<UInt32>(material.flags.depthFlags)]);

		UInt32 culling = GL_BACK;
		
		if (material.flags.cullingMode == CullingMode::Both)
		{
			culling = 0;
		}
		else if (material.flags.cullingMode == CullingMode::Front)
		{
			culling = GL_FRONT;
		}

		if (culling != 0)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(culling);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void GraphicsDevice::SetTextureFlags(Texture& texture)
	{
		static const UInt32 GLFilters[] = {
			GL_NEAREST,
			GL_LINEAR,
			GL_NEAREST_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR,
			GL_LINEAR_MIPMAP_LINEAR,
		};

		static const UInt32 GLWraps[] = {
			GL_REPEAT,
			GL_CLAMP_TO_EDGE
		};

		const UInt32 mag = static_cast<UInt32>(texture.flags.magFiltering);
		UInt32 min = static_cast<UInt32>(texture.flags.minFiltering);

		if (texture.flags.mipmaps)
		{
			if (min == 0 && mag == 0)
			{
				min = 2;
			}
			else if (min == 1 && mag == 0)
			{
				min = 3;
			}
			else if (min == 0 && mag == 1)
			{
				min = 4;
			}
			else if (min == 1 && mag == 1)
			{
				min = 5;
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLFilters[min]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLFilters[mag]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLWraps[static_cast<UInt32>(texture.flags.wrapModes)]);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLWraps[static_cast<UInt32>(texture.flags.wrapModes)]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GLWraps[static_cast<UInt32>(texture.flags.wrapModes)]);
	}
}
