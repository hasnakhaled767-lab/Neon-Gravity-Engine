#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

struct NeonParticle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius;
    float mass;
    sf::Color coreColor;
};

float getDistance(sf::Vector2f a, sf::Vector2f b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

sf::Color getRandomNeonColor() {
    int palette = std::rand() % 4;
    switch (palette) {
    case 0: return sf::Color(0, 240, 255);
    case 1: return sf::Color(255, 0, 128);
    case 2: return sf::Color(138, 43, 226);
    default: return sf::Color(0, 255, 160);
    }
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const unsigned int WIDTH = 1200;
    const unsigned int HEIGHT = 750;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Neon Gravity & Particle Physics Engine");
    window.setFramerateLimit(60);

    std::vector<NeonParticle> particles;

    auto spawnParticles = [&](float x, float y, int count) {
        for (int i = 0; i < count; ++i) {
            NeonParticle p;
            p.pos = { x, y };
            float angle = (std::rand() % 360) * 3.14159f / 180.f;
            float speed = 2.0f + static_cast<float>(std::rand() % 60) / 10.f;
            p.vel = { std::cos(angle) * speed, std::sin(angle) * speed };
            p.radius = 4.0f + static_cast<float>(std::rand() % 8);
            p.mass = p.radius * 0.5f;
            p.coreColor = getRandomNeonColor();
            particles.push_back(p);
        }
    };

    spawnParticles(WIDTH / 2.f, HEIGHT / 2.f, 60);

    float gravityY = 0.15f;
    float damping = 0.88f;
    bool gravityEnabled = true;
    float mouseForce = 1200.f;

    sf::RectangleShape fadeOverlay(sf::Vector2f(WIDTH, HEIGHT));
    fadeOverlay.setFillColor(sf::Color(10, 14, 26, 45));

    sf::CircleShape shape;

    window.clear(sf::Color(10, 14, 26));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    gravityEnabled = !gravityEnabled;
                }
                if (event.key.code == sf::Keyboard::C) {
                    particles.clear();
                }
                if (event.key.code == sf::Keyboard::R) {
                    particles.clear();
                    spawnParticles(WIDTH / 2.f, HEIGHT / 2.f, 80);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i mPos = sf::Mouse::getPosition(window);
                spawnParticles(static_cast<float>(mPos.x), static_cast<float>(mPos.y), 25);
            }
        }

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));
        bool isAttracting = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        for (size_t i = 0; i < particles.size(); ++i) {
            auto& p = particles[i];

            if (gravityEnabled) {
                p.vel.y += gravityY;
            }

            if (isAttracting) {
                sf::Vector2f dir = mousePos - p.pos;
                float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (dist > 15.f && dist < 600.f) {
                    sf::Vector2f normDir = dir / dist;
                    float pull = mouseForce / (dist * 1.5f);
                    p.vel += normDir * (pull * 0.05f);
                }
            }

            p.vel *= 0.996f;
            p.pos += p.vel;

            if (p.pos.x - p.radius < 0.f) {
                p.pos.x = p.radius;
                p.vel.x = -p.vel.x * damping;
            }
            else if (p.pos.x + p.radius > WIDTH) {
                p.pos.x = WIDTH - p.radius;
                p.vel.x = -p.vel.x * damping;
            }

            if (p.pos.y - p.radius < 0.f) {
                p.pos.y = p.radius;
                p.vel.y = -p.vel.y * damping;
            }
            else if (p.pos.y + p.radius > HEIGHT) {
                p.pos.y = HEIGHT - p.radius;
                p.vel.y = -p.vel.y * damping;
            }
        }

        window.draw(fadeOverlay);

        sf::VertexArray lines(sf::Lines);
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                float d = getDistance(particles[i].pos, particles[j].pos);
                if (d < 95.f) {
                    sf::Uint8 alpha = static_cast<sf::Uint8>((1.f - d / 95.f) * 140);
                    sf::Color lineCol = particles[i].coreColor;
                    lineCol.a = alpha;
                    lines.append(sf::Vertex(particles[i].pos, lineCol));
                    lines.append(sf::Vertex(particles[j].pos, lineCol));
                }
            }
        }
        window.draw(lines, sf::BlendAdd);

        for (const auto& p : particles) {
            shape.setRadius(p.radius * 2.2f);
            shape.setOrigin(p.radius * 2.2f, p.radius * 2.2f);
            shape.setPosition(p.pos);
            sf::Color glow = p.coreColor;
            glow.a = 45;
            shape.setFillColor(glow);
            window.draw(shape, sf::BlendAdd);

            shape.setRadius(p.radius);
            shape.setOrigin(p.radius, p.radius);
            shape.setPosition(p.pos);
            shape.setFillColor(p.coreColor);
            window.draw(shape);
        }

        if (isAttracting) {
            sf::CircleShape blackHole(22.f);
            blackHole.setOrigin(22.f, 22.f);
            blackHole.setPosition(mousePos);
            blackHole.setFillColor(sf::Color(255, 255, 255, 30));
            blackHole.setOutlineThickness(2.f);
            blackHole.setOutlineColor(sf::Color(0, 240, 255, 200));
            window.draw(blackHole, sf::BlendAdd);
        }

        window.display();
    }

    return 0;
}