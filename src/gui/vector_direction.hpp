#pragma once
#include <SFML/Graphics.hpp>
#include "engine/common/math.hpp"


struct VectorDirection : public sf::Drawable
{
    sf::Vector2f position;
    sf::Vector2f direction;
    float        length   = 0.0f;
    float        radius   = 0.0f;
    sf::Color    color    = sf::Color::White;

    VectorDirection() = default;
    VectorDirection(float r, float l)
        : length(l)
        , radius(r)
    {}

    void setAngle(float a)
    {
        direction = {cos(a), sin(a)};
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(position);
        circle.setFillColor(color);
        target.draw(circle, states);

        
        const float A = radius + length;
        const float c = sqrt(A * A - radius * radius);
        const float a = Math::PI * 0.5f - acos(c / A);
        const sf::Vector2f r1{ cos(a), sin(a) };
        const sf::Vector2f r2{ r1.x, -r1.y };

        sf::VertexArray va(sf::Quads, 4);
        va[0].position = position;
        va[1].position = position + MathVec2::rotateDir(r1, direction) * radius;
        va[2].position = position + direction * (radius + length);
        va[3].position = position + MathVec2::rotateDir(r2, direction) * radius;
        va[0].color = color;
        va[1].color = color;
        va[2].color = color;
        va[3].color = color;
        target.draw(va, states);
    }
};
