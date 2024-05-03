#ifndef BOIDS
#define BOIDS

#include <SFML/Graphics.hpp>
#include <vector>
#include <cassert>

namespace uc
{
    class boid
    {
    private:
        // body serve a generare il boid a schermo e per trovarne la posizione
        sf::RectangleShape body;

        // vettore velocità
        sf::Vector2f velocity{0, 0};

        // vettore accelerazione
        sf::Vector2f acceleration{0, 0};

        // larghezza schermo
        float width{0};

        // altezza schermo
        float height{0};

        // angolo di orientazione del boid
        double direction{0};

        //tipo di boid (se è uguale a 99, allora è un predatore) 
        int boidType{};

    public:
        // COSTRUTTORE
        boid(float r, sf::Vector2f pos, float w, float h, int boidType);

        //Ritorna un vettore con i boid vicini e le distanze medie
        std::tuple<std::vector<uc::boid>, std::vector<float>, std::vector<float>> closer_boids(std::vector<boid> const boids, float sight_distance);
        
        //calcola tutte e tre le componenti di velocità che verranno aggiunte
        sf::Vector2f acs(std::vector<boid> boids, float a, float c, float s, float separation_distance);

        //aggiunge una componente di velocità per allontanare i boid dai bordi
        sf::Vector2f border_separation(float max_speed);

        //aggiunge una componente di velocità per allontanare i boid dai predatori
        sf::Vector2f escape_predators(std::vector<boid> boids, float eP);

        //aggiunge una componente di velocità ai predatori per inseguire i boid
        sf::Vector2f follow_preys(std::vector<boid> boids_red);

        //aggiorna la direzione di tutti i boid
        float new_direction(sf::Vector2f velocity);
        
        //aggiorna la velocità di tutti i boid
        sf::Vector2f new_velocity(std::vector<boid> const boids_red, float a, float c, float s, float separation_distance, float max_speed);

        //aggiorna la posizione di tutti i boid
        void update_position(sf::Vector2f position);

        //GET
        auto const get_body()
        {
            return body;
        }
        auto const get_velocity()
        {
            return velocity;
        }
        int get_boid_type()
        {
            return boidType;
        }

        //SET
        void set_direction(float steerangle)
        {
            body.rotate(steerangle);
        }
        void set_velocity(sf::Vector2f const n_vel)
        {
            velocity = n_vel;
        }
        void set_color(sf::Color colore)
        {
            body.setFillColor(colore);
        }
    };
    //restituisce tutti i valori di velocità e distanze medie tra boid dei due stormi, con relative deviazioni standard
    std::tuple<sf::Vector2f, sf::Vector2f, float, float, sf::Vector2f, sf::Vector2f, float, float> print_stats(std::vector<uc::boid> boids, float n1, float n2, std::vector<float> allDistances1, std::vector<float> allDistances2, std::ofstream *stats);
    
    //funzione che calcola la deviazione standard
    float dev_standard(std::vector<float> x, float u);

    //funzione che genera i boids sullo schermo
    std::vector<boid> create_boids (int n1, int n2, int n3, float width, float height);
}
#endif
