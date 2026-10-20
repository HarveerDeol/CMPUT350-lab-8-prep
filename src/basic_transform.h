#ifndef BASIC_TRANSFORM_H
#define BASIC_TRANSFORM_H

#include <cassert> 
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    BasicTransform(const sf::Vector2f& scale = {1.f, 1.f},
                   const sf::Vector2f& translation = {0.f, 0.f})
        : scale(scale), translation(translation) {}

    sf::Vector2f applyPoint(const sf::Vector2f& p) const { return p * scale + translation; }
    sf::Vector2f applyVector(const sf::Vector2f& v) const { return v * scale; }

    bool operator==(const BasicTransform& other) const {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};

#endif