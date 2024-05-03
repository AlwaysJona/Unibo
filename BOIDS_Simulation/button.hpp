#ifndef BUTTON
#define BUTTON

#include <SFML/Graphics.hpp>
#include "boids.hpp"
#include <cassert>
#include <iostream>
namespace bt
{

    class button
    {
    private:
        sf::RectangleShape buttonShape;
        float width{0};
        float height{0};
        sf::Vector2i position;

        sf::Font font;
        sf::Text text;
        std::string textContent;
        sf::Vector2f textPosition;
        sf::Color color;

    public:
        button(float width, float height, sf::Vector2f position, std::string textContent, sf::Color color);
        auto const get_shape()
        {
            return buttonShape;
        }
        auto const get_position()
        {
            return position;
        }
        auto const get_width()
        {
            return width;
        }
        auto const get_height()
        {
            return height;
        }
        void set_text_position(sf::Vector2f newposition)
        {
            text.setPosition(newposition);
        }
        bool is_button_clicked(sf::Vector2f mousePosition);
        void draw(sf::RenderWindow &window)
        {
            window.draw(buttonShape);
            window.draw(text);
        }
    };

    class slider
    {
    private:
        sf::RectangleShape sliderShape;
        float width{0};
        float height{0};
        sf::Vector2i position;
        float parameter{0.5};
        float hitbox{0}; // parameter is in pixel

        int needpointer{1}; // if needpointer == 0 no pointer is displayed
        sf::CircleShape pointerShape;

        sf::Font font;
        sf::Text text;
        std::string textContent;

    public:
        slider(float width, float height, sf::Vector2f position, float parameter, float hitbox, int needpointer, std::string textContent);

        auto const get_shape()
        {
            return sliderShape;
        }
        auto const get_position()
        {
            return position;
        }
        auto const get_width()
        {
            return width;
        }
        auto const get_height()
        {
            return height;
        }
        auto const get_hitbox_position()
        { // TODO!! somewhere i need to check if hitbox position is outside the window
            return sf::Vector2f(position.x - hitbox, position.y - hitbox);
        }
        auto const get_hitbox_width()
        {
            return (width + 2 * hitbox);
        }
        auto const get_hitbox_height()
        {
            return (height + 2 * hitbox);
        }
        auto const get_parameter()
        {
            return parameter;
        }
        void set_parameter(float newParameter)
        {
            parameter = newParameter;
        }

        bool is_slider_clicked(sf::Vector2f mousePosition);

        void clicked(int clickedPoint);

        auto const get_pointer_shape()
        {
            return pointerShape;
        }
        void update_pointer_position();
        template <typename t>
        void set_text(t newtext)
        {
            text.setString(std::to_string(newtext));
        }
        void draw(sf::RenderWindow &window)
        {
            window.draw(sliderShape);
            window.draw(pointerShape);
            window.draw(text);
        }
    };
}

#endif
