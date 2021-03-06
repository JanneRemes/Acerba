#pragma once
 
#include <Ace/Vector2.h>

#include <Ace/Sprite.h>
#include <Ace/SpriteSheet.h>

#include <Ace/Path.h>

#include <memory>
#include <vector>



// TEMP
#include <tmxlite/Map.hpp>

namespace ace
{
    struct Drawable
    {
        /**
            @brief Custom Draw method
            Requires manual buffer or sprite logic.
        */
		virtual void Draw() const = 0;
    };


    class Tilemap : public Drawable
    {
    public:

		typedef void(*ReadTilemap)(Sprite& sprite, UInt32 id, UInt32 layer, void* arg);

        struct TileLayer : public Drawable
        {
            std::vector<Sprite> tiles;

            virtual void Draw() const;
        };

    private:

        struct TiledImpl;
        std::shared_ptr<TiledImpl> m_tiledImpl;

    public:

        Texture tileset;

        Tilemap(const Path& map, float scale = 1.0, const Vector3& pivot = Vector3(0.5, 0.5, 0), ReadTilemap callback = nullptr, void* arg = nullptr);
        ~Tilemap();

        UInt32 LayersCount() const;
        TileLayer* GetLayer(Int32 i);

        SpriteSheet& GetSpriteSheet();
        const SpriteSheet& GetSpriteSheet() const;

        virtual void Draw() const;

		tmx::Map& GetMap();

        operator bool() const;
    };
}