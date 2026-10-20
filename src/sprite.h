#ifndef SPRITE_H
#define SPRITE_H

#include <array>
#include <cstddef>

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture)
        : Sprite(texture, {0.f, 0.f}, sf::Vector2f(texture.getSize())) {}

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint)
        : mTexture(&texture),
          mVertexArray(sf::PrimitiveType::Triangles, 6),
          mMinPointTex(texMinPoint),
          mMaxPointTex(texMaxPoint),
          mOrigin(0.f, 0.f) {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!mTexture) return;
        recomputeVertexArray();
        states.texture = mTexture;
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {{
        {0.f, 0.f}, {0.f, 1.f}, {1.f, 0.f},  
        {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}   
    }};

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {
        if (!mTexture) return;
        const sf::Vector2f texSize = mMaxPointTex - mMinPointTex;
        for (std::size_t i = 0; i < DEFAULT_VERTICES.size(); ++i) {
            const sf::Vector2f local = DEFAULT_VERTICES[i] * texSize;  // size in texture pixels
            mVertexArray[i].texCoords = mMinPointTex + local;
            mVertexArray[i].position = mTransform.applyPoint(local - mOrigin);
        }
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif