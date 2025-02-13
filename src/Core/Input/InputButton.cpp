#include <vili/ErrorHandler.hpp>

#include <Input/InputButton.hpp>
#include <SFML/Window/Joystick.hpp>

namespace obe::Input
{
    std::ostream& operator<<(std::ostream& os, const AxisCompareType& m)
    {
        (m == AxisCompareType::LESS) ? os << "LESS" : os << "MORE";
        return os;
    }

    InputButton::InputButton(sf::Keyboard::Key key, const std::string& name,
                             const std::string& returnChar, InputType type)
    {
        m_button = key;
        m_name = name;
        m_returnChar = returnChar;
        m_type = type;
    }

    InputButton::InputButton(sf::Mouse::Button key, const std::string& name)
    {
        m_button = key;
        m_name = name;
        m_returnChar = "";
        m_type = InputType::Mouse;
    }

    InputButton::InputButton(unsigned int gamepadIndex,
                             unsigned int buttonIndex, const std::string& name)
    {
        m_gamepadIndex = gamepadIndex;
        m_button = buttonIndex;
        m_returnChar = "";
        m_type = InputType::GamepadButton;
        m_name = name;
    }

    InputButton::InputButton(unsigned int gamepadIndex,
                             sf::Joystick::Axis gamepadAxis,
                             std::pair<AxisCompareType, float> detect,
                             const std::string& name)
    {
        m_gamepadIndex = gamepadIndex;
        m_type = InputType::GamepadAxis;
        m_button = gamepadAxis;
        m_returnChar = "";
        m_name = name;
        m_detectAxis = detect;
    }

    sf::Keyboard::Key InputButton::getKey() const
    {
        return std::get<sf::Keyboard::Key>(m_button);
    }

    std::string InputButton::getName() const
    {
        return m_name;
    }

    InputType InputButton::getType() const
    {
        return m_type;
    }

    bool InputButton::is(InputType inputType) const
    {
        return inputType == m_type;
    }

    bool InputButton::isWritable() const
    {
        return (!m_returnChar.empty());
    }

    bool InputButton::isPressed() const
    {
        if (m_type == InputType::Mouse)
            return sf::Mouse::isButtonPressed(
                std::get<sf::Mouse::Button>(m_button));
        if (m_type == InputType::GamepadButton)
            return sf::Joystick::isButtonPressed(
                m_gamepadIndex, std::get<unsigned int>(m_button));
        if (m_type == InputType::GamepadAxis)
        {
            const float axisValue = sf::Joystick::getAxisPosition(
                m_gamepadIndex, std::get<sf::Joystick::Axis>(m_button));
            return (m_detectAxis.first == AxisCompareType::LESS)
                       ? axisValue < m_detectAxis.second
                       : axisValue > m_detectAxis.second;
        }

        return sf::Keyboard::isKeyPressed(
            std::get<sf::Keyboard::Key>(m_button));
    }

    unsigned int InputButton::getAxisPosition()
    {
        if (m_type == InputType::GamepadAxis)
            return sf::Joystick::getAxisPosition(
                m_gamepadIndex, std::get<sf::Joystick::Axis>(m_button));
        else
            throw aube::ErrorHandler::Raise("Obe.Input.InputButton.NotAnAxis");
    }
} // namespace obe::Input
