#include "button.hpp"
#include "boids.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

const int desktopWidth = sf::VideoMode::getDesktopMode().width;

bt::button::button(float width, float height, sf::Vector2f position, std::string textContent, sf::Color color) : width{width}, height{height}, position{position}, textContent{textContent}, color{color}
{
    buttonShape.setSize(sf::Vector2f(width, height));
    buttonShape.setFillColor(color);
    buttonShape.setPosition(position);

    assert(((void)"Unable to load font, maybe font.ttf is missing?", font.loadFromFile("font.ttf")));
    font.loadFromFile("font.ttf");
    text.setFont(font);
    text.setString(textContent);
    text.setCharacterSize(round(desktopWidth/45));
    text.setFillColor(sf::Color(30, 36, 66));
    text.setStyle(sf::Text::Italic);
    text.setLetterSpacing(9);
    const sf::FloatRect bounds(text.getLocalBounds());
    const sf::Vector2f box(buttonShape.getSize());
    text.setOrigin((bounds.width - box.x) / 2 + bounds.left, (bounds.height - box.y) / 2 + bounds.top);

    text.setPosition(position.x, position.y);
}

bool bt::button::is_button_clicked(sf::Vector2f mousePosition){
    if ((this->get_position().x < mousePosition.x) and (mousePosition.x < this->get_position().x + this->get_width()) and (this->get_position().y < mousePosition.y) and (mousePosition.y < this->get_position().y + this->get_height())){
        return true;
    } else {
        return false;
    }
}



bt::slider::slider(float width, float height, sf::Vector2f position, float parameter, float hitbox, int needpointer, std::string textContent) : width{width}, height{height}, position{position}, parameter{parameter}, hitbox{hitbox}, needpointer{needpointer}, textContent{textContent}
{
    sliderShape.setSize(sf::Vector2f(width, height));
    sliderShape.setFillColor(sf::Color(30, 36, 66));
    sliderShape.setPosition(position);

    if (needpointer==1){
        pointerShape.setRadius(3*height/2);
        pointerShape.setFillColor(sf::Color(30, 36, 66));
        pointerShape.setPosition(sf::Vector2f(position.x + parameter * width - 3*height/2/*=pointer radius*/, position.y + height / 2 - 3*height/2/*=pointer radius*/));
    }

    assert(((void)"Unable to load font, maybe font.ttf is missing?", font.loadFromFile("font.ttf")));
    font.loadFromFile("font.ttf");
    text.setFont(font);
    text.setString(textContent);
    text.setCharacterSize(desktopWidth/100);
    text.setFillColor(sf::Color(30, 36, 66));
    text.setStyle(sf::Text::Italic);
    text.setPosition(position.x + get_hitbox_width(), position.y - desktopWidth/100/2 /* = character size/2 */);
}

bool bt::slider::is_slider_clicked(sf::Vector2f mousePosition){
    if ((this->get_hitbox_position().x < mousePosition.x) and (mousePosition.x < this->get_hitbox_position().x + this->get_hitbox_width()) and (this->get_hitbox_position().y < mousePosition.y) and (mousePosition.y < this->get_hitbox_position().y + this->get_hitbox_height()))
    {
        return true;
    } else {
        return false;
    }
}
void bt::slider::update_pointer_position()
{
    pointerShape.setPosition(sf::Vector2f(position.x + parameter * width - 3*height/2/*=pointer radius*/, position.y + height / 2 - 3*height/2/*=pointer radius*/));
}

void bt::slider::clicked(int clickedPoint)
{
    this->set_parameter((clickedPoint) / this->get_hitbox_width());
}
