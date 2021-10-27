#pragma once
#include <SFML/Graphics.hpp>


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

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(position);
        circle.setFillColor(color);

        sf::VertexArray va(sf::Triangles, 3);
    }
};
