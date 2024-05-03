#include <cstdlib>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include "boids.hpp"
#include "vector_algebra.hpp"

//COSTRUTTORE
uc::boid::boid(float r, sf::Vector2f pos, float w, float h, int boidType) : width{w}, height{h}, boidType{boidType}
{
    body.setSize(sf::Vector2f(r, r / 2));
    body.setOrigin(sf::Vector2f(r / 2, r / 4));
    body.setFillColor(sf::Color::Black);
    body.setPosition(pos);
}
//************************************************************************
std::tuple<std::vector<uc::boid>, std::vector<float>, std::vector<float>> uc::boid::closer_boids(std::vector<uc::boid> boids, float sight_distance)
{
    std::vector<uc::boid> closer_b;
    std::vector<float> distances1;
    std::vector<float> distances2;

    //posizione del boid corrente
    sf::Vector2f pos = body.getPosition();

    for (auto boid : boids)
    {
        //posizione del boid selezionato
        sf::Vector2f c_pos = boid.get_body().getPosition();
        //distanza tra il boid corrente e quello selezionato
        float distance = va::distance(pos, c_pos);
        //il boid viene aggiunto se è distante meno di sight_distance
        if (distance <= sight_distance && pos != c_pos)
        {
            closer_b.push_back(boid);
        }

        //se hanno lo stesso tipo, il boid selezionato viene aggiunto alla lista di distanze corrispondente
        if (boidType == 1 and boid.get_boid_type() == 1)
        {
            distances1.push_back(distance);
        }
        else if (boidType == 2 and boid.get_boid_type() == 2)
        {
            distances2.push_back(distance);
        }
    }
    return {closer_b, distances1, distances2};
}
//************************************************************************
sf::Vector2f uc::boid::acs(std::vector<uc::boid> boids, float a, float c, float s, float separation_distance)
{
    //le costanti hanno proporzioni diverse, per ottenere una simulazione più piacevole
    a = 8 * a;
    c = 0.1 * c;
    s = 5 * s;

    //vettori delle 3 componenti
    sf::Vector2f tot_v_align{0, 0};
    sf::Vector2f tot_v_cohe{0, 0};
    sf::Vector2f tot_v_sep{0, 0};

    //posizione del centro di massa relativo
    sf::Vector2f tot_p_cohe{0, 0};

    //variabili che controllano che venga controllato almeno un boid
    float tot_sep = 0;
    float tot_ac = 0;

    for (auto boid : boids)
    {
        //allineamento e coesione sfruttano lo stesso loop
        if (boid.get_boid_type() == boidType)
        {
            tot_v_align += boid.get_velocity();
            tot_p_cohe += boid.get_body().getPosition();
            tot_ac++;
            //Separazione
            float dx = body.getPosition().x - boid.get_body().getPosition().x;
            float dy = body.getPosition().y - boid.get_body().getPosition().y;

            //Se il boid osservato è più vicino di un valore separation_distance, viene sommata una componente che tende a respingerli
            if (dx >= 0 and dx < separation_distance)
            {
                tot_v_sep.x += -dx + separation_distance;
                tot_sep++;
            
            } else if (dx < 0 and (-dx) < separation_distance)
            {   
                tot_v_sep.x += -separation_distance-dx;
                tot_sep++;
            }
            if (dy>=0 and dy < separation_distance)
            {
                tot_v_sep.y += -dy+separation_distance;
                tot_sep++;
            } else if (dy<0 and (-dy) < separation_distance)
            {
                tot_v_sep.y += -separation_distance-dy;
                tot_sep++;
            }
        }

        
    }
    //se i loop sono stati controllati almeno una volta, allora fai la media delle velocità
    if (tot_ac > 0)
    {
        tot_v_align /= tot_ac;
        tot_p_cohe /= tot_ac;
        tot_v_cohe = (tot_p_cohe - body.getPosition());
    }
    if (tot_sep > 0)
    {
        tot_v_sep /= tot_sep;
    }

    return a*tot_v_align + c*tot_v_cohe + tot_v_sep*s;
}
//************************************************************************
sf::Vector2f uc::boid::border_separation(float max_speed)
{
    sf::Vector2f position = body.getPosition();
    sf::Vector2f tot_v = velocity;
    // se il boid si trova vicino al bordo destro, aggiungi una componente negativa alla velocità x
    if ((position.x + 10 * max_speed) > width)
    {                                                    
        tot_v.x -= 2 * max_speed * (position.x / width);
    }
    // se il boid si trova vicino al bordo sinistro aggiungi una componente positiva alla velocità x
    else if ((position.x - 10 * max_speed) < 0)
    {                                                          
        tot_v.x += 2 * max_speed * (1 - (position.x / width));
    }
    // se il boid si trova vicino al bordo inferiore aggiungi una componente negativa alla velocità x
    if ((position.y + 10 * max_speed) > height)
    { 
        tot_v.y -= 2 * max_speed * (position.y / height);
    }
    // se il boid si trova vicino al bordo superiore aggiungi una componente positiva alla velocità x
    else if ((position.y - 10 * max_speed) < 0)
    {                                                           
        tot_v.y += 2 * max_speed * (1 - (position.y / height));
    }

    // nella parte precedente il boid veniva allontanato con forza proporzionale alla distanza dal bordo, ma a volte
    // capita che il boid (specialmente se inseguito da un predatore) riesca comunque a superare il bordo. Questa parte
    // fa si che il boid vada a "sbattere" col bordo e non possa superarlo in alcun modo.
    if ((position.x + max_speed) > width)
    {
        tot_v.x = -max_speed;
    }
    else if ((position.x - max_speed) < 0)
    {
        tot_v.x = max_speed;
    }
    if ((position.y + max_speed) > height)
    {
        tot_v.y = -max_speed;
    }
    else if ((position.y - max_speed) < 0)
    {
        tot_v.y = max_speed;
    }
    return (tot_v);
}
//************************************************************************
sf::Vector2f uc::boid::escape_predators(std::vector<uc::boid> neighbors, float eP)
{
    float tot = 0;
    sf::Vector2f tot_v{0, 0};
    for (auto neighbor : neighbors)
    {
        //se il boid selezionato è un predatore, crea una componente di velocità che aumenta con l'avvicinarsi del predatore
        if (neighbor.get_boid_type() == 99)
        {
            float d = va::distance(body.getPosition(), neighbor.get_body().getPosition());
            sf::Vector2f diff = body.getPosition() - neighbor.get_body().getPosition();
            tot_v += diff / d;
            assert(((void)"tot_v is NaN", diff / d == diff / d));
            tot++;
        }
    }
    //se controlla almeno una volta, fai la media
    if (tot > 0)
    {
        tot_v /= tot;
    }

    return eP * tot_v;
}
//************************************************************************
sf::Vector2f uc::boid::follow_preys(std::vector<uc::boid> neighbours)
{
    float tot = 0;
    sf::Vector2f tot_p{0, 0};
    sf::Vector2f tot_v{0, 0};
    for (auto neighbour : neighbours)
    {
        //se il tipo di boid del vicino è diverso dal proprio, lo insegue
        if (neighbour.boidType != boidType)
        { 
            tot_p += neighbour.get_body().getPosition();
            tot++;
        }
    }
    //se controlla almeno una volta fa la media
    if (tot > 0)
    {
        tot_p /= tot;
        tot_v = (tot_p - body.getPosition());
    }
    return (tot_v);
}
//************************************************************************
float uc::boid::new_direction(sf::Vector2f velocity)
{
    float newdirection = atan2(velocity.y, velocity.x) * 180 / M_PI;
    float actualdirection = body.getRotation();
    float steerangle = newdirection - actualdirection;

    // trova il percorso di rotazione più breve (l'arcotangente restituisce valori solo tra -90° e +90°)
    if (steerangle > 180)
    {
        steerangle = 360 - steerangle;
    }
    else if (steerangle < -180)
    {
        steerangle = 360 + steerangle;
    }
    return (steerangle);
}
//************************************************************************
sf::Vector2f uc::boid::new_velocity(std::vector<uc::boid> const neighbors, float a, float c, float s, float separation_distance, float max_speed)
{
    // l'accelerazione di ogni boid è la somma delle 3 componenti, più una terza per scappare dai predatori
    if (get_boid_type() != 99)
    {
        acceleration = acs(neighbors, a, c, s, separation_distance) + escape_predators(neighbors, 20);
    }
    //l'accelerazione dei predatori è data solo dalla separation e il termine per inseguire le prede
    else if (get_boid_type() == 99)
    {
        acceleration = acs(neighbors, 0, 0, 10, separation_distance) + follow_preys(neighbors);
    }

    velocity += acceleration;
    velocity = border_separation(max_speed);

    //setta un limite alla velocità massima, per evitare che accelerino all'infinito
    va::limit(velocity, max_speed);
    assert(((void)"velocity is NaN", velocity.x == velocity.x));
    assert(((void)"velocity is NaN", velocity.y == velocity.y));
    return (velocity);
}
//************************************************************************
void uc::boid::update_position(sf::Vector2f velocity)
{
    float n_x = body.getPosition().x + velocity.x;
    float n_y = body.getPosition().y + velocity.y;

    assert(((void)"n_x is NAN", n_x == n_x));
    assert(((void)"n_y is NAN", n_y == n_y));
    assert(((void)"n_x is negative", n_x >= 0));
    assert(((void)"n_y is negative", n_y >= 0));

    body.setPosition(sf::Vector2f(n_x, n_y));
}
//************************************************************************
std::tuple<sf::Vector2f, sf::Vector2f, float, float, sf::Vector2f, sf::Vector2f, float, float> uc::print_stats(std::vector<uc::boid> boids, float n1, float n2, std::vector<float> allDistances1, std::vector<float> allDistances2, std::ofstream *stats)
{
    //distanze totali
    float totD1{0};
    float totD2{0};

    //velocità totali
    sf::Vector2f totV1{0, 0};
    sf::Vector2f totV2{0, 0};

    std::vector<float> allVelocities1x;
    std::vector<float> allVelocities1y;
    std::vector<float> allVelocities2x;
    std::vector<float> allVelocities2y;

    for (auto d : allDistances1)
    {
        totD1 += d;
    }
    for (auto d : allDistances2)
    {
        totD2 += d;
    }

    for (auto boid : boids)
    {
        //se è del primo stormo, aggiunge le velocità a totV1 e allVelocities1
        if (boid.get_boid_type() == 1)
        {
            totV1.x += boid.get_velocity().x;
            totV1.y += boid.get_velocity().y;
            allVelocities1x.push_back(boid.get_velocity().x);
            allVelocities1y.push_back(boid.get_velocity().y);
        }
        //se è del secondo stormo, aggiunge a totV2 e allVelocities2
        if (boid.get_boid_type() == 2)
        {
            totV2.x += boid.get_velocity().x;
            totV2.y += boid.get_velocity().y;
            allVelocities2x.push_back(boid.get_velocity().x);
            allVelocities2y.push_back(boid.get_velocity().y);
        }
    }
    //Medie e deviazioni standard
    float meanD1{totD1 / (n1 * n1)};
    float meanD2{totD2 / (n2 * n2)};
    float devD1 = dev_standard(allDistances1, meanD1);
    float devD2 = dev_standard(allDistances2, meanD2);

    sf::Vector2f meanV1{totV1.x / (n1), totV1.y / (n1)};
    sf::Vector2f meanV2{totV2.x / (n2), totV2.y / (n2)};
    sf::Vector2f devV1 = {dev_standard(allVelocities1x, meanV1.x), dev_standard(allVelocities1y, meanV1.y)};
    sf::Vector2f devV2 = {dev_standard(allVelocities2x, meanV2.x), dev_standard(allVelocities2y, meanV1.y)};
    
    //se le medie non sono dei -nan (quindi non vengono calcolate), allora non vengono aggiunte a statistics.txt
    if(meanV1.x==meanV1.x){
        *stats << "(" << meanV1.x << " +- " << devV1.x << " , " << meanV1.y << " +- " << devV1.y << ")\t\t";
        *stats << meanD1 << " +- " << devD1 << "\t\t";
    }
    if (meanV2.x==meanV2.x){
        *stats << "(" << meanV2.x << " +- " << devV2.x << " , " << meanV2.y << " +- " << devV2.y << ")\t\t";
        *stats << meanD2 << " +- " << devD2 << "\t\t";
    }
    *stats<<'\n';
    return {meanV1, devV1, meanD1, devD1, meanV2, devV2, meanD2, devD2};
}
//************************************************************************
float uc::dev_standard(std::vector<float> x, float u)
{
    float sum{0};
    long unsigned int N{x.size()};
    sum = std::accumulate(x.begin(), x.end(), 0.0, [&u](float sum, float x)
                          { return sum + std::pow((x - u), 2); });
    return (std::sqrt(sum / N));
}
//************************************************************************
std::vector<uc::boid> uc::create_boids(int n1, int n2, int n3, float width, float height)
{
    //lista contenente tutti i boids
    std::vector<uc::boid> boids;

    //valori per ricevere dei numeri randomici in base ad una distribuzione casuale di valori
    std::default_random_engine engine;
    std::uniform_real_distribution<float> w_distribution(0, width);
    std::uniform_real_distribution<float> h_distribution(0, height);
    std::uniform_real_distribution<float> vel_distribution(-5, 5);

    //Loop per generare il primo stormo
    for (int i = 0; i < static_cast<int>(n1); i++)
    {
        uc::boid boid_1(round(width / 240), sf::Vector2f(w_distribution(engine), h_distribution(engine)), width, height, 1);
        auto velocity = (sf::Vector2f(vel_distribution(engine), vel_distribution(engine)));
        boid_1.set_velocity(velocity);
        boid_1.set_direction(boid_1.new_direction(velocity));
        boid_1.set_color(sf::Color(244, 239, 230));
        boids.push_back(boid_1);
    }
    //Loop per generare il secondo stormo
    for (int i = 0; i < static_cast<int>(n2); i++)
    {
        uc::boid boid_2(round(width / 240), sf::Vector2f(w_distribution(engine), h_distribution(engine)), width, height, 2);
        auto velocity_2 = (sf::Vector2f(vel_distribution(engine), vel_distribution(engine)));
        boid_2.set_velocity(velocity_2);
        boid_2.set_direction(boid_2.new_direction(velocity_2));
        boid_2.set_color(sf::Color(28, 78, 79));
        boids.push_back(boid_2);
    }
    //Loop per generare i predatori
    for (int i = 0; i < static_cast<int>(n3); i++)
    { 
        uc::boid predator(round(width / 120), sf::Vector2f(w_distribution(engine), h_distribution(engine)), width, height, 99);
        auto velocity = (sf::Vector2f(vel_distribution(engine), vel_distribution(engine)));
        predator.set_velocity(velocity);
        predator.set_direction(predator.new_direction(velocity));
        predator.set_color(sf::Color(208, 95, 76));
        boids.push_back(predator);
    }
    return boids;
}
