#ifndef V_ALGEBRA
#define V_ALGEBRA

#include <SFML/Graphics.hpp>

namespace va
{
    // distanza tra due vettori
    float distance(const sf::Vector2f &a, const sf::Vector2f &b);

    // modulo
    float magnitude(const sf::Vector2f &vec);

    // normalizza ad 1
    void normalize(sf::Vector2f &vec);

    // se il modulo è troppo alto, normalizza ad un valore limite
    void limit(sf::Vector2f &vec, const float limit);
}
#endif
