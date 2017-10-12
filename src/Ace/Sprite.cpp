#include <Ace/Sprite.h>
#include <Ace/Math.h>


namespace ace
{

    const ace::Vertex triangle[4] = {
        { { 0.5f, 0.5f, 0, 0.0f }, { 1.0f, 0.0f }, 0xFFFFFFFFU },
        { { -0.5f, 0.5f, 0, 0.0f }, { 0.0f, 0.0f }, 0xFFFFFFFFU },
        { { -0.5f, -0.5f, 0, 0.0f }, { 0.0f, 1.0f }, 0xFFFFFFFFU },
        { { 0.5f, -0.5f, 0, 0.0f }, { 1.0f, 1.0f }, 0xFFFFFFFFU },
    };

    Sprite::Sprite() : Sprite(triangle)
    {	

    }

	Sprite::Sprite(float deg) : Sprite()
	{
		const math::Matrix4 rot(math::Matrix4::RotationZ(deg));
		for (auto& i : vertexData)
		{
			i.position = rot * i.position;
		}
	}

    Sprite::Sprite(const Vertex(&data)[4]) :
        vertexData{{data[0], data[1], data[2], data[3]}}
    {
		
    }

    Sprite::Sprite(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) :
        vertexData{{ v1, v2, v3, v4 }}
    {

    }


	void Sprite::SetSprite(const SpriteSheet::SpriteData* sprite)
	{
		if (sprite == nullptr)
		{
			return;
		}

		Texcoord(sprite->texcoord);
	}

	void Sprite::SetSprite(const SpriteSheet::SpriteData* sprite, float scale, float base)
	{
		if (sprite == nullptr)
		{
			return;
		}

		Texcoord(sprite->texcoord);
		Scale(Vector2(sprite->location.width / base, sprite->location.height / base) * scale);

	}

	void Sprite::Rotate(float deg)
	{
		const math::Matrix4 rot(math::Matrix4::RotationZ(deg));
		for (auto& i : vertexData)
		{
			i.position = rot * i.position;
		}
	}

	void Sprite::Scale(const Vector2& scale)
	{
        for (int i = 0; i < Sprite::size; ++i)
        {
            vertexData[i].position.x = triangle[i].position.x * scale.x;
            vertexData[i].position.y = triangle[i].position.y * scale.y;
        }
	}

    void Sprite::Scale(const Texture& texture, float scale)
    {
        for (int i  = 0; i < Sprite::size; ++i)
        {
            vertexData[i].position.x = triangle[i].position.x * scale * texture.size.x / texture.scale;
            vertexData[i].position.y = triangle[i].position.y * scale * texture.size.y / texture.scale;
        }
    }

	void Sprite::Move(const Vector3& pos)
	{
		for(auto& i : vertexData)
		{
			i.position.x += pos.x;
            i.position.y += pos.y;
            i.position.z += pos.z;
		}
	}

	void Sprite::Colorize(const Color32& color)
	{
		for (auto& i : vertexData)
		{
			i.color = color;
		}
	}

	void Sprite::Texcoord(const Rect& uv)
	{
			vertexData[1].uv.x = uv.x;
			vertexData[0].uv.y = uv.y;
			
			vertexData[0].uv.x = uv.x + uv.width;
			vertexData[1].uv.y = uv.y;

			vertexData[3].uv.x = uv.x + uv.width;
			vertexData[2].uv.y = uv.y + uv.height;

			vertexData[2].uv.x = uv.x;
			vertexData[3].uv.y = uv.y + uv.height;
	}

	void Sprite::FlipUV(const bool axis)
	{
		Sprite temp = *this;

		if (axis)
		{
			vertexData[0].uv.x = temp.vertexData[2].uv.x;
			vertexData[1].uv.x = temp.vertexData[3].uv.x;
			vertexData[2].uv.x = temp.vertexData[0].uv.x;
			vertexData[3].uv.x = temp.vertexData[1].uv.x;
		}
		else
		{
			vertexData[0].uv.y = temp.vertexData[2].uv.y;
			vertexData[1].uv.y = temp.vertexData[3].uv.y;
			vertexData[2].uv.y = temp.vertexData[0].uv.y;
			vertexData[3].uv.y = temp.vertexData[1].uv.y;
		}
	}

	void Sprite::SetColor(UInt8 index, const Color32& color)
	{
		vertexData[index].color = color;
	}

	void Sprite::SetUV(UInt8 index, const Vector2& uv)
	{
		vertexData[index].uv = uv;
	}

	void Sprite::SetPosition(UInt8 index, const Vector2& positon)
	{
		vertexData[index].position.x = positon.x;
		vertexData[index].position.y = positon.y;
	}

	void Sprite::SetInstanceID(UInt8 id)
	{
		vertexData[0].position.w = id;
		vertexData[1].position.w = id;
		vertexData[2].position.w = id;
		vertexData[3].position.w = id;
	}

}