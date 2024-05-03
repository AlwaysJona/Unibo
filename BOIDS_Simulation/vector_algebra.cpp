#include <SFML/Graphics.hpp>
#include <cmath>
#include "vector_algebra.hpp"
//Calcola la distanza tra due vettori
float va::distance(const sf::Vector2f &a, const sf::Vector2f &b)
{
    return static_cast<float>(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2)));
}
//calcola il modulo di un vettore
float va::magnitude(const sf::Vector2f &vec)
{
    return static_cast<float>(std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2)));
}
//setta il modulo di un vettore ad 1
void va::normalize(sf::Vector2f &vec)
{
    float mag = va::magnitude(vec);
    if (mag > 0)
    {
        vec /= mag;
    }
}
//imposta un limite al modulo del vettore
void va::limit(sf::Vector2f &vec, const float limit)
{
    float mag = magnitude(vec);
    if (mag > 0 && mag > limit)
    {
        normalize(vec);
        vec *= limit;
    }
}
