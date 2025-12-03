#pragma once

#include <SFML/Graphics.hpp>
#include <string>

inline sf::RectangleShape createButton(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color fill,
    sf::Color outline = sf::Color::Black,
    float outlineThickness = 4.f)
{
    sf::RectangleShape button(size);
    button.setPosition(position);
    button.setFillColor(fill);
    button.setOutlineColor(outline);
    button.setOutlineThickness(outlineThickness);
    return button;
}

inline sf::Text createText(
    const sf::Font& font,
    const std::string& str,
    unsigned int size,
    sf::Vector2f position,
    sf::Color fill = sf::Color::White,
    sf::Color outline = sf::Color::Black,
    float outlineThickness = 2.f)
{
    sf::Text text(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(fill);
    text.setOutlineColor(outline);
    text.setOutlineThickness(outlineThickness);
    text.setPosition(position);
    return text;
}