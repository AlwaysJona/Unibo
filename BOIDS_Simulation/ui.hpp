#ifndef UI
#define UI

#include "button.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <chrono>

namespace ui
{

    // Controlla se il pulsante START è premuto
    int check_button_start(bt::button *buttonStart, bt::slider *parameter1, bt::slider *parameter2, sf::Vector2f &mousePosition)
    {
        int n{0};
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (buttonStart->is_button_clicked(mousePosition))
            {
                if (parameter1->get_parameter() != 0 or parameter2->get_parameter() != 0)
                {
                    n = 1;
                }
                else
                {
                    std::cout << "We need at least 1 boid!";
                }
            }
        }
        return n;
    }
    // Controlla i pulsanti "+"
    void check_plus_hitboxes(std::vector<bt::button *> plus_hitboxes, std::vector<bt::slider *> parameters, sf::Vector2f &mousePosition)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (unsigned int i = 0; i < plus_hitboxes.size(); i++)
            {
                if (plus_hitboxes[i]->is_button_clicked(mousePosition))
                {
                    int par = parameters[i]->get_parameter();
                    parameters[i]->set_parameter(par + 1);
                    parameters[i]->set_text<int>(par + 1);
                }
            }
        }
    }
    // Controlla i pulsanti "-"
    void check_minus_hitboxes(std::vector<bt::button *> minus_hitboxes, std::vector<bt::slider *> parameters, sf::Vector2f &mousePosition)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (unsigned int i = 0; i < minus_hitboxes.size(); i++)
            {
                if (minus_hitboxes[i]->is_button_clicked(mousePosition))
                {
                    int par = parameters[i]->get_parameter();
                    if (par != 0)
                    {
                        parameters[i]->set_parameter(par - 1);
                        parameters[i]->set_text<int>(par - 1);
                    }
                    else
                    {
                        std::cout << "\nThis parameter cannot be negative.";
                    }
                }
            }
        }
    }
    // Controlla gli slider
    void check_sliders(std::vector<bt::slider *> sliders, sf::Vector2f mousePosition)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (unsigned int i = 0; i < sliders.size(); i++)
            {
                if (sliders[i]->is_slider_clicked(mousePosition))
                {
                    sliders[i]->clicked(mousePosition.x - sliders[i]->get_hitbox_position().x);
                    sliders[i]->update_pointer_position();
                }
            }
        }
    }
}

#endif
