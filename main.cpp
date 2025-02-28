#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Structure to hold quiz questions and answers
struct Question {
    std::string question;
    std::vector<std::string> options;
    int correctOption; // Index of the correct option
};

// Function to initialize quiz questions
std::vector<Question> initializeQuestions() {
    return {
        {"What is the capital of France?", {"London", "Paris", "Berlin", "Madrid"}, 1},
        {"What is 2 + 2?", {"3", "4", "5", "6"}, 1},
        {"What is the largest planet in the solar system?", {"Earth", "Mars", "Jupiter", "Saturn"}, 2},
        {"What is the chemical symbol for water?", {"CO2", "H2O", "O2", "NaCl"}, 1},
        {"Who wrote 'Romeo and Juliet'?", {"Shakespeare", "Hemingway", "Tolstoy", "Dickens"}, 0}
    };
}

int main() {
    // Initialize SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Quiz Game with AI");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Initialize health
    int userHealth = 100;
    int aiHealth = 100;

    // Initialize quiz questions
    std::vector<Question> questions = initializeQuestions();
    srand(static_cast<unsigned int>(time(0)));

    // Current question index
    int currentQuestionIndex = 0;

    // Text objects
    sf::Text questionText;
    questionText.setFont(font);
    questionText.setCharacterSize(24);
    questionText.setFillColor(sf::Color::White);
    questionText.setPosition(50, 50);

    // Health bar properties
    const int healthBarWidth = 200;
    const int healthBarHeight = 20;
    const int healthBarSpacing = 40;

    sf::RectangleShape userHealthBar(sf::Vector2f(healthBarWidth, healthBarHeight));
    userHealthBar.setFillColor(sf::Color::Green);
    userHealthBar.setPosition(50, 400);

    sf::RectangleShape aiHealthBar(sf::Vector2f(healthBarWidth, healthBarHeight));
    aiHealthBar.setFillColor(sf::Color::Green);
    aiHealthBar.setPosition(50, 400 + healthBarSpacing);

    // Health bar backgrounds
    sf::RectangleShape userHealthBarBackground(sf::Vector2f(healthBarWidth, healthBarHeight));
    userHealthBarBackground.setFillColor(sf::Color::Red);
    userHealthBarBackground.setPosition(50, 400);

    sf::RectangleShape aiHealthBarBackground(sf::Vector2f(healthBarWidth, healthBarHeight));
    aiHealthBarBackground.setFillColor(sf::Color::Red);
    aiHealthBarBackground.setPosition(50, 400 + healthBarSpacing);

    // Button properties
    const int buttonWidth = 200;
    const int buttonHeight = 50;
    const int buttonSpacing = 20;
    const int buttonStartY = 150;

    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonLabels;

    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(50, buttonStartY + i * (buttonHeight + buttonSpacing));
        button.setFillColor(sf::Color::Blue);
        buttons.push_back(button);

        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(20);
        label.setFillColor(sf::Color::White);
        label.setPosition(60, buttonStartY + i * (buttonHeight + buttonSpacing) + 10);
        buttonLabels.push_back(label);
    }

    // Main game loop
    while (window.isOpen() && userHealth > 0 && aiHealth > 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    // Check which button was clicked
                    for (size_t i = 0; i < buttons.size(); ++i) {
                        if (buttons[i].getGlobalBounds().contains(mousePos)) {
                            // Check if the answer is correct
                            if (i == questions[currentQuestionIndex].correctOption) {
                                aiHealth -= 10; // AI loses health
                            } else {
                                userHealth -= 10; // User loses health
                            }

                            // AI's turn (randomly correct or incorrect)
                            bool aiCorrect = (rand() % 2 == 0);
                            if (aiCorrect) {
                                userHealth -= 10; // User loses health
                            } else {
                                aiHealth -= 10; // AI loses health
                            }

                            // Move to the next question
                            currentQuestionIndex = (currentQuestionIndex + 1) % questions.size();
                            break;
                        }
                    }
                }
            }
        }

        // Update question text
        questionText.setString("Question: " + questions[currentQuestionIndex].question);

        // Update button labels
        for (size_t i = 0; i < buttonLabels.size(); ++i) {
            buttonLabels[i].setString(questions[currentQuestionIndex].options[i]);
        }

        // Update health bars
        userHealthBar.setSize(sf::Vector2f(healthBarWidth * (userHealth / 100.0f), healthBarHeight));
        aiHealthBar.setSize(sf::Vector2f(healthBarWidth * (aiHealth / 100.0f), healthBarHeight));

        // Clear the window
        window.clear();

        // Draw everything
        window.draw(questionText);
        window.draw(userHealthBarBackground);
        window.draw(aiHealthBarBackground);
        window.draw(userHealthBar);
        window.draw(aiHealthBar);
        for (const auto& button : buttons) {
            window.draw(button);
        }
        for (const auto& label : buttonLabels) {
            window.draw(label);
        }

        // Display the window
        window.display();
    }

    // Game over message
    if (userHealth <= 0) {
        std::cout << "You lost! The AI wins." << std::endl;
    } else if (aiHealth <= 0) {
        std::cout << "You win! The AI loses." << std::endl;
    }

    return 0;
}
