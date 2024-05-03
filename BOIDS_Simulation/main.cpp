#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "boids.hpp"
#include "button.hpp"
#include "ui.hpp"
#include <fstream>
#include <tuple>
#include <cmath>
#include <chrono>
#include <thread>
int main()
{
    // Altezza e larghezza del desktop
    unsigned const int desktopWidth = sf::VideoMode::getDesktopMode().width;
    unsigned const int desktopHeight = sf::VideoMode::getDesktopMode().height;
    unsigned const int width = round(desktopWidth / 2);
    unsigned const int height = round(desktopHeight / 2);

    // Renderizza la finestra e la centra nello schermo
    auto window = sf::RenderWindow(sf::VideoMode(width, height), "Boids simulation");
    window.setPosition(sf::Vector2i(width / 2, height / 2));

    // Il vettore di boids contiene i boids di tutti gli stormi, compresi i predatori
    std::vector<uc::boid> boids;

    // Pulsante START
    bt::button buttonStart(width / 3, height / 7, sf::Vector2f(width / 2 - width / 6, 2 * height / 3), "START", sf::Color(0, 0, 0, 0));

    // Pulsanti "+" e "-", boidNumber1 e boidNumber2 contengono il numero di boid dei due stormi
    bt::button plus1_hitbox(width / 60, width / 60, sf::Vector2f(width / 2 + 150, height / 2 - 4 * height / 12), "+", sf::Color(0, 0, 0, 0));
    bt::button minus1_hitbox(width / 60, width / 60, sf::Vector2f(width / 2 + 120, height / 2 - 4 * height / 12), "-", sf::Color(0, 0, 0, 0));
    bt::slider boidNumber1(0, 0, sf::Vector2f(width / 2 + 200, height / 2 - 4 * height / 12), 200, 0, 0, "200");
    bt::slider text1(0, 0, sf::Vector2f(width / 4, height / 2 - 4 * height / 12), 0, 0, 0, "Number of boids of the first flock:");

    bt::button plus2_hitbox(width / 60, width / 60, sf::Vector2f(width / 2 + 150, height / 2 - 3 * height / 12), "+", sf::Color(0, 0, 0, 0));
    bt::button minus2_hitbox(width / 60, width / 60, sf::Vector2f(width / 2 + 120, height / 2 - 3 * height / 12), "-", sf::Color(0, 0, 0, 0));
    bt::slider boidNumber2(0, 0, sf::Vector2f(width / 2 + 200, height / 2 - 3 * height / 12), 0, 0, 0, "0");
    bt::slider text2(0, 0, sf::Vector2f(width / 4, height / 2 - 3 * height / 12), 0, 0, 0, "Number of boids of the second flock:");

    // predatorNumber contiene il numero di predatori
    bt::button plus3_hitbox(width / 60, width / 60, sf::Vector2f(width / 2 + 150, height / 2 - 2 * height / 12), "+", sf::Color(0, 0, 0, 0));
    bt::button minus3_hitbox(width / 60, width / 60, sf::Vector2f(width / 2 + 120, height / 2 - 2 * height / 12), "-", sf::Color(0, 0, 0, 0));
    bt::slider predatorNumber(0, 0, sf::Vector2f(width / 2 + 200, height / 2 - 2 * height / 12), 0, 0, 0, "0");
    bt::slider text3(0, 0, sf::Vector2f(width / 4, height / 2 - 2 * height / 12), 0, 0, 0, "Number of predators:");

    // Mostra il numero di fps
    bt::slider fpsNumber(0, 0, sf::Vector2f(18 * width / 20, 1 * height / 20), 0, 0, 0, "0");
    bt::slider fpsText(0, 0, sf::Vector2f(19 * width / 20, 1 * height / 20), 0, 0, 0, "FPS");

    // Ogni slider ritorna un parametro tra 0 e 1 che andrà moltiplicato alla velocità
    bt::slider aligSlider(width / 14, height / 110, sf::Vector2f(width / 100, height / 24), 0.5, width / 110, 1, "Alignment");
    bt::slider coheSlider(width / 14, height / 110, sf::Vector2f(width / 100, height / 12), 0.5, width / 110, 1, "Cohesion");
    bt::slider sepSlider(width / 14, height / 110, sf::Vector2f(width / 100, height / 8), 0.5, width / 110, 1, "Separation");
    bt::slider distSlider(width / 14, height / 110, sf::Vector2f(width / 100, height - height / 24), 0.5, width / 110, 1, "Separation distance");
    bt::slider sightSlider(width / 14, height / 110, sf::Vector2f(width / 100, height - height / 12), 0.5, width / 110, 1, "Sight distance");
    bt::slider speedSlider(width / 14, height / 110, sf::Vector2f(width / 100, height - height / 8), 0.5, width / 110, 1, "Max speed");

    // frameCounter serve a numerare le iterazioni dell'event loop
    unsigned int frameCounter{0};
    float steerangle{0.};

    // Vettori contenenti le distanze relative tra i boid, servirà a calcolare la deviazione standard
    std::vector<float> allDistances1;
    std::vector<float> allDistances2;

    // apri il file statistics.txt
    std::ofstream stats("statistics.txt");
    stats << "Velocità media stormo 1:\tDistanza media stormo1:\tVelocità media stormo 2:\tDistanza media stormo 2:\n";

    /******EVENT LOOP******/
    while (window.isOpen())
    {
        // Evento CLOSE
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                stats.close();
                window.close();
            }
        }

        // Posizione del mouse relativa al sistema di riferimento della finestra
        sf::Vector2i pixelMousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosition = window.mapPixelToCoords(pixelMousePosition);

        // colore della finestra
        window.clear(sf::Color(100, 162, 163));

        // se frameCounter è zero, mostra la schermata iniziale
        if (frameCounter == 0)
        {
            // controllo ogni 15 millisecondi, ver rendere meno sensibili i controlli
            std::this_thread::sleep_for(std::chrono::milliseconds(15));

            // se il bottone start viene cliccato, framecounter assumerà valore 1 e si andranno a simulare i boid
            frameCounter = ui::check_button_start(&buttonStart, &boidNumber1, &boidNumber2, mousePosition);

            // vettori contenendi i pulsanti "+", "-" e i parametri
            std::vector<bt::button *> vec_of_plus{&plus1_hitbox, &plus2_hitbox, &plus3_hitbox};
            std::vector<bt::slider *> vec_of_par{&boidNumber1, &boidNumber2, &predatorNumber};
            std::vector<bt::button *> vec_of_minus{&minus1_hitbox, &minus2_hitbox, &minus3_hitbox};

            // controlla che i pulsanti "+" e "-" vengano cliccati e quindi aumenta o diminuisce i vari parametri
            ui::check_plus_hitboxes(vec_of_plus, vec_of_par, mousePosition);
            ui::check_minus_hitboxes(vec_of_minus, vec_of_par, mousePosition);

            // colora il pulsante START
            auto shape = buttonStart.get_shape();
            shape.setFillColor(sf::Color(100, 100, 100));

            // DRAW
            buttonStart.draw(window);
            plus1_hitbox.draw(window);
            minus1_hitbox.draw(window);
            plus2_hitbox.draw(window);
            minus2_hitbox.draw(window);
            plus3_hitbox.draw(window);
            minus3_hitbox.draw(window);
            boidNumber1.draw(window);
            boidNumber2.draw(window);
            predatorNumber.draw(window);
            text1.draw(window);
            text2.draw(window);
            text3.draw(window);
        }

        // se il frameCounter è 1, crea i boids
        if (frameCounter == 1)
        {
            boids = uc::create_boids(boidNumber1.get_parameter(), boidNumber2.get_parameter(), predatorNumber.get_parameter(), width, height);
            frameCounter = 2;
        }

        // se frameCounter è maggiore di 1, parte la simulazione
        if (frameCounter > 1)
        {
            // Controlla che tutti gli sliders vengano cliccati
            std::vector<bt::slider *> vec_of_sliders{&aligSlider, &coheSlider, &sepSlider, &distSlider, &sightSlider, &speedSlider};
            ui::check_sliders(vec_of_sliders, mousePosition);

            // start prende il tempo di inizio calcolo degli fps
            auto start = std::chrono::high_resolution_clock::now();

            // DRAW
            aligSlider.draw(window);
            coheSlider.draw(window);
            sepSlider.draw(window);
            distSlider.draw(window);
            sightSlider.draw(window);
            speedSlider.draw(window);
            fpsText.draw(window);
            fpsNumber.draw(window);

            frameCounter += 1;

            // Aggiorna la posizione di ogni boids
            for (int i = 0; i < std::int16_t(boids.size()); i++)
            {
                // tuple contenente il vettore di boid vicini (neighbours) e delle distanze medie
                auto [neighbours, distances1, distances2] = boids[i].closer_boids(boids, sightSlider.get_parameter() * (width / 10));
                allDistances1.insert(allDistances1.end(), distances1.begin(), distances1.end());
                allDistances2.insert(allDistances2.end(), distances2.begin(), distances2.end());

                // la funzione new_velocity() modifica la velocità del boid, prendendo come input neighbours e i parametri restituiti dagli slider,
                // tramite la funzione slider.get_parameter() (i parametri vengono "immagazzinati" negli slider).
                auto v = boids[i].new_velocity(neighbours, aligSlider.get_parameter(), coheSlider.get_parameter(), sepSlider.get_parameter(), distSlider.get_parameter() * (width / 50), speedSlider.get_parameter() * 10 + 1);

                // aggiorna la posizione del boid rispetto alla nuova velocità.
                boids[i].update_position(v);

                // ogni 3 iterazioni aggiorna l'angolo di virata, per una maggiore fluidità dei movimenti
                if (frameCounter % 3 == 0)
                {
                    steerangle = boids[i].new_direction(v);
                }

                // gira il boid dell'angolo di virata diviso per 3, sempre per evitare che si girino di scatto
                boids[i].set_direction(steerangle / 3);
                // disegna i boids
                window.draw(boids[i].get_body());
            }

            // print_stats restituisce tutte le velocità e distanze medie con le relative deviazioni standard
            auto [v1, dev_v1, d1, dev_d1, v2, dev_v2, d2, dev_d2] = uc::print_stats(boids, boidNumber1.get_parameter(), boidNumber2.get_parameter(), allDistances1, allDistances2, &stats);
            // ogni 100 iterazioni stampa a video le statistiche
            if (frameCounter == 102)
            {
                std::cout << "\n******** STATISTICHE ********\n";
                if (v1.x == v1.x)
                {
                    std::cout << "\n  -Velocità media primo tipo: (" << v1.x << " +- " << dev_v1.x << " , " << v1.y << " +- " << dev_v1.y << ")px/frame;";
                    std::cout << "\n  -Distanza media primo tipo: (" << d1 << " +- " << dev_d1 << ")px;";
                    std::cout << "\n";
                }
                if (v2.x == v2.x)
                {
                    std::cout << "  -Velocità media secondo tipo: (" << v2.x << " +- " << dev_v2.x << " , " << v2.y << " +- " << dev_v2.y << ")px/frame;";
                    std::cout << "\n  -Distanza media secondo tipo: (" << d2 << " +- " << dev_d2 << ")px;";
                }
                std::cout << "\n*****************************";
                std::cout << "\n";
                frameCounter = 2;
            }

            // Resetta i vettori con le distanze medie
            allDistances1.clear();
            allDistances2.clear();

            // stop prende il tempo di fine calcolo degli fps
            auto stop = std::chrono::high_resolution_clock::now();

            // Calcola il numero di fps
            auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            int fps = round((10E5 / frameTime.count()));
            if (frameCounter % 5 == 0)
            {
                fpsNumber.set_text<int>(fps);
            }
        }
        window.display();
    }
}
