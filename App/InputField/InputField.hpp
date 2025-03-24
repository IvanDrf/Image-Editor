#include <SFML/Graphics.hpp>

namespace {
const short KEnter{13};
const short kBackSpace{8};
}  // namespace

class InputField {
 private:
    sf::RectangleShape box_;
    sf::Font font_;
    sf::Text text_;
    std::string inputString_;

 public:
    InputField();
    InputField(const float x, const float y, const sf::Vector2f& size);

    void SetTextPosition(const float x, const float y);
    void SetFont(const sf::Font& font);
    void SetText(const std::string& text);
    void SetTextColor(const sf::Color& color);
    void SetTextSize(const int textSize);

    void SetInputText(const std::string& inputString);

    void SetBoxPosition(const float x, const float y);
    void SetBoxSize(const sf::Vector2f& size);
    void SetBoxColor(const sf::Color& color);

    bool Active(const sf::Vector2f& mousePosition) const;

    std::string Input(sf::Event& event, sf::RenderWindow& window);

    static std::string CreateInputWindow(const std::string& heading, const std::string& inputText);

    void Clear();
    void Draw(sf::RenderWindow& window) const;
};
