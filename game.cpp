#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 0.5f;
const float JUMP_STRENGTH = -10.f;

class Bird {
public:
    Bird() {
        if (!textureLoaded) {
            if (!texture.loadFromFile("bird.png")) {
                std::cerr << "ERROR: Could not load bird texture." << std::endl;
            }
            textureLoaded = true; 
        }
        sprite.setTexture(texture);
        sprite.setPosition(100.f, WINDOW_HEIGHT / 2.f);
        sprite.setScale(0.08f, 0.08f);
        velocity = 0.f;
    }

    void update(bool& gameOver) {
        if (gameOver) return; // Prevent updates if the game is over

        velocity += GRAVITY;
        sprite.move(0.f, velocity);

        // Prevent going off screen
        if (sprite.getPosition().y < 0) {
            sprite.setPosition(sprite.getPosition().x, 0);
            velocity = 0.f;
        }
        // Check if bird touches the floor
        if (sprite.getPosition().y + sprite.getGlobalBounds().height >= WINDOW_HEIGHT) {
            sprite.setPosition(sprite.getPosition().x, WINDOW_HEIGHT - sprite.getGlobalBounds().height);
            velocity = 0.f;
            gameOver = true; // Set game over state
        }
    }

    void jump() {
        velocity = JUMP_STRENGTH;
    }

    sf::Sprite sprite;

private:
    static sf::Texture texture;
    static bool textureLoaded; 
    float velocity;
};

sf::Texture Bird::texture;
bool Bird::textureLoaded = false;

class Pipe {
public:
    Pipe(float x) {
        float gapPosition = rand() % (WINDOW_HEIGHT - 200) + 100.f;
        float gapSize = 200.f;

        // Top pipe
        topPipe.setSize(sf::Vector2f(50.f, gapPosition));
        topPipe.setFillColor(sf::Color::Green);
        topPipe.setPosition(x, 0.f);

        // Bottom pipe
        bottomPipe.setSize(sf::Vector2f(50.f, WINDOW_HEIGHT - (gapPosition + gapSize)));
        bottomPipe.setFillColor(sf::Color::Green);
        bottomPipe.setPosition(x, gapPosition + gapSize);
    }

    void move(float speed) {
        topPipe.move(-speed, 0.f);
        bottomPipe.move(-speed, 0.f);
    }

    sf::RectangleShape topPipe;
    sf::RectangleShape bottomPipe;
};

int main() {
    // Seed random number generator
    srand(time(nullptr));

    // Create the game window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy Bird");
    window.setFramerateLimit(60);

    // Create bird and pipes
    Bird bird;
    std::vector<Pipe> pipes;
    for (int i = 0; i < 3; ++i) {
        pipes.emplace_back(WINDOW_WIDTH + i * 300.f);
    }
    int score = 0;
    bool gameOver = false;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "ERROR: Could not load font." << std::endl;
        return -1;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(WINDOW_WIDTH - 100, 10); // Position at the top right

    // Game loop
    while (window.isOpen()) {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Jump on space key
            if (!gameOver && event.type == sf::Event::KeyPressed && 
                event.key.code == sf::Keyboard::Space) {
                bird.jump();
            }

            // Restart game on Enter key
            if (gameOver && event.type == sf::Event::KeyPressed && 
                event.key.code == sf::Keyboard::Enter) {
                // Reset game state
                bird = Bird();
                pipes.clear();
                for (int i = 0; i < 3; ++i) {
                    pipes.emplace_back(WINDOW_WIDTH + i * 300.f);
                }
                score = 0;
                gameOver = false;
            }
        }

        if (!gameOver) {
            // Update bird
            bird.update(gameOver);

            // Update and recycle pipes
            for (auto& pipe : pipes) {
                pipe.move(5.f);

                // If pipe is off screen, reset its position
                if (pipe.topPipe.getPosition().x < -50.f) {
                    pipes.erase(pipes.begin());
                    pipes.emplace_back(WINDOW_WIDTH);
                    score++; // Increment score when a pipe is passed
                }
            }

            // Collision detection (simplified)
            for (auto& pipe : pipes) {
                if (bird.sprite.getGlobalBounds().intersects(pipe.topPipe.getGlobalBounds()) ||
                    bird.sprite.getGlobalBounds().intersects(pipe.bottomPipe.getGlobalBounds())) {
                    gameOver = true; // Set game over state
                }
            }

            // Update score text
            scoreText.setString("Score: " + std::to_string(score));
        }

        // Clear and draw
        window.clear(sf::Color::Cyan);
        
        // Draw pipes
        for (auto& pipe : pipes) {
            window.draw(pipe.topPipe);
            window.draw(pipe.bottomPipe);
        }

        // Draw bird
        window.draw(bird.sprite);

        // Draw score
        window.draw(scoreText);

        // If game is over, display final score
        if (gameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(48);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setString("Game Over!");
            gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50); // Centered

            sf::Text scoreTextGameOver;
            scoreTextGameOver.setFont(font);
            scoreTextGameOver.setCharacterSize(24);
            scoreTextGameOver.setFillColor(sf::Color::Black);
            scoreTextGameOver.setString("Score: " + std::to_string(score));
            scoreTextGameOver.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2); // Below Game Over text

            window.draw(gameOverText);
            window.draw(scoreTextGameOver);

            sf::Text playAgainText;
            playAgainText.setFont(font);
            playAgainText.setCharacterSize(24);
            playAgainText.setFillColor(sf::Color::Black);
            playAgainText.setString("Press Enter to Play Again");
            playAgainText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 30); // Below score text
            window.draw(playAgainText);
        }

        // Display
        window.display();
    }

    return 0;
}