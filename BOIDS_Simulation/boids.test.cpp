#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "boids.hpp"

TEST_CASE("Testing")
{
    float width{900};
    float height{600};
    std::vector<uc::boid> boids;

    SUBCASE("Testing the create_boids function")
    {
        float type1{0}, type2{0}, type3{0};
        boids = uc::create_boids(110, 120, 10, width, height);

        for (auto boid : boids)
        {
            if (boid.get_boid_type() == 1)
            {
                type1 += 1;
            }
            else if (boid.get_boid_type() == 2)
            {
                type2 += 1;
            }
            else
            {
                type3 += 1;
            }
        }

        CHECK(boids.size() == 240);
        CHECK(type1 == 110);
        CHECK(type2 == 120);
        CHECK(type3 == 10);
    }

    SUBCASE("Testing the acs function")
    {
        boids.clear();
        uc::boid boid1(round(width / 240), sf::Vector2f(110, 100), width, height, 1);
        uc::boid boid2(round(width / 240), sf::Vector2f(120, 100), width, height, 1);
        boids.push_back(boid1);
        boids.push_back(boid2);

        // testing cohesion
        auto acs1 = boid1.acs(boids, 0, 10, 0, 10);
        auto acs2 = boid2.acs(boids, 0, 10, 0, 10);
        CHECK(acs1.x == 5);
        CHECK(acs1.y == 0);
        CHECK(acs2.x == -5);
        CHECK(acs2.y == 0);

        // testing separation
        acs1 = boid1.acs(boids, 0, 0, 1, 10);
        acs2 = boid2.acs(boids, 0, 0, 1, 10);
        CHECK(acs1.x == -8);
        CHECK(acs1.y == 0);
        CHECK(acs2.x == -acs1.x);
        CHECK(acs1.y == 0);

        // testing alignment
        boids.clear();
        boid1.set_velocity(sf::Vector2f(10, 0));
        boids.push_back(boid1);
        boids.push_back(boid2);
        acs1 = boid1.acs(boids, 1, 0, 0, 10);
        acs2 = boid2.acs(boids, 1, 0, 0, 10);
        CHECK(acs2.x == 5);
    }

    uc::boid boid1{6, sf::Vector2f{100, 100}, width, height, 1};
    uc::boid boid2{6, sf::Vector2f{110, 100}, width, height, 1};
    uc::boid boid3{6, sf::Vector2f{100, 90}, width, height, 2};
    uc::boid boid4{6, sf::Vector2f{110, 90}, width, height, 2};

    std::ofstream stats("statistics.txt");

    /*
    SUBCASE("Testing the update_velocity function")
    {
        auto v1 = boid1.new_velocity(flock2, 1, 1, 1, 10, 10);
        auto v2 = boid2.new_velocity(flock1, 1, 1, 1, 10, 10);

        CHECK(v1.x == -v2.x);
        CHECK(v1.y == v2.y);
    }
    SUBCASE("Testing the set_velocity function")
    {
        sf::Vector2f vel{5, 5};
        boid1.set_velocity(vel);
        CHECK(boid1.get_velocity() == vel);
    }
    SUBCASE("Testing the print_stats function")
    {
        sf::Vector2f V1{5, 0};
        sf::Vector2f V2{0, 5};
        sf::Vector2f V3{0, 10};
        sf::Vector2f V4{10, 0};

        boid1.set_velocity(V1);
        boid2.set_velocity(V2);
        boid3.set_velocity(V3);
        boid4.set_velocity(V4);

        boids.push_back(boid1);
        boids.push_back(boid2);
        boids.push_back(boid3);
        boids.push_back(boid4);

        std::vector<float> d1i {40, 40, 40, 40};
        std::vector<float> d2i{40, 40, 40, 40};

        auto [v1, dev_v1, d1, dev_d1, v2, dev_v2, d2, dev_d2] = uc::print_stats(boids, 40, 40, d1i, d2i, &stats);

        CHECK(v1.x == 2.5);
        CHECK(v1.y == 2.5);
        CHECK(v2.x == 5);
        CHECK(v2.y == 5);

        CHECK(dev_v1.x == 2.5);
        CHECK(dev_v1.y == 2.5);
        CHECK(dev_v2.x == 5);
        CHECK(dev_v2.y == 5);

        CHECK(d1 == 10);
        CHECK(d2 == 10);

        CHECK(dev_d1 == 6);
        CHECK(dev_d2 == 6);
    }*/
}
/*
std::cout << "\nDistanze totali 1: " << totalDistances1.size();
std::cout << "\nDistanze totali 2: " << totalDistances2.size();*/
