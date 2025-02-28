#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Structure to hold quiz questions and answers
struct Question {
    std::string question;
    std::string answer;
};

// Function to initialize quiz questions
std::vector<Question> initializeQuestions() {
    return {
        {"What is the capital of France?", "Paris"},
        {"What is 2 + 2?", "4"},
        {"What is the largest planet in the solar system?", "Jupiter"},
        {"What is the chemical symbol for water?", "H2O"},
        {"Who wrote 'Romeo and Juliet'?", "Shakespeare"}
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

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(50, 150);

    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(50, 400);

    // Main game loop
    while (window.isOpen() && userHealth > 0 && aiHealth > 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle text input
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !inputText.getString().isEmpty()) {
                    // Backspace: remove last character
                    std::string currentText = inputText.getString();
                    currentText.pop_back();
                    inputText.setString(currentText);
                } else if (event.text.unicode < 128) {
                    // Append character to input
                    inputText.setString(inputText.getString() + static_cast<char>(event.text.unicode));
                }
            }

            // Handle Enter key (submit answer)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                std::string userAnswer = inputText.getString();
                std::string correctAnswer = questions[currentQuestionIndex].answer;

                // Check if answer is correct
                if (userAnswer == correctAnswer) {
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
                inputText.setString(""); // Clear input
            }
        }

        // Update question text
        questionText.setString("Question: " + questions[currentQuestionIndex].question);

        // Update health text
        healthText.setString("Your Health: " + std::to_string(userHealth) + "\nAI Health: " + std::to_string(aiHealth));

        // Clear the window
        window.clear();

        // Draw everything
        window.draw(questionText);
        window.draw(inputText);
        window.draw(healthText);

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
