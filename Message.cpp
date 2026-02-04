#include "Message.h"

#include <array>
#include <cstring>

// Constructor
Message::Message(MessagePayload payload) : m_payload(std::move(payload))
{
    // Set m_format based on the payload type
    std::visit([this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, WheelMessage>)
            m_format = MESSAGE_FORMAT_WHEEL;
        else if constexpr (std::is_same_v<T, ArmMessage>)
            m_format = MESSAGE_FORMAT_ARM;
        else if constexpr (std::is_same_v<T, ScienceToolMessage>)
            m_format = MESSAGE_FORMAT_SCIENCE_TOOL;
        else
            m_format = static_cast<MessageFormat>(MESSAGE_FORMAT_GENERIC); // Or a GENERIC/UNKNOWN value if you have one
    }, m_payload);
}

// Default Constructor
Message::Message() : m_payload(Generic{0}), m_format(static_cast<MessageFormat>(MESSAGE_FORMAT_GENERIC)) {}

// Copy Constructor
Message::Message(Message const& src) : m_payload(src.m_payload), m_format(src.m_format) { }

// Destructor
Message::~Message() { }

// Assignment Operator
Message& Message::operator=(const Message& src) {
    if (this != &src) {
        m_format = src.m_format;
        m_payload = src.m_payload;
    }
    return *this;
}

// Get the format of the message
MessageFormat Message::getFormat() const { return m_format; }

// Print Message details
void Message::printMessage() const {
    std::visit(
        [this](auto&& payload) {
            using T = std::decay_t<decltype(payload)>;
            if constexpr (std::is_same_v<T, Generic>) {
                std::cout << "Generic - Value: " << payload.value;
            } else if constexpr (std::is_same_v<T, WheelMessage>) {
                std::cout << "WheelMessage - Velocity: " << payload.velocity
                          << ", Theta: " << payload.theta
                          << ", Angle Velocity: " << payload.angle_velocity;
            } else if constexpr (std::is_same_v<T, ArmMessage>) {
                std::cout << "ArmMessage - X: " << payload.armXPos
                          << ", Y: " << payload.armYPos
                          << ", Z: " << payload.armZPos
                          << ", Claw X: " << payload.clawXPos
                          << ", Claw Y: " << payload.clawYPos
                          << ", Claw Open: " << payload.clawOpen
                          << ", Claw Rotation: " << payload.clawRotation
                          << ", Wrist Rotation: " << payload.wristRotation;
            } else if constexpr (std::is_same_v<T, ScienceToolMessage>) {
                std::cout << "ScienceToolMessage - Move Up/Down: " << payload.moveUpDown
                          << ", Move Left/Right: " << payload.moveLeftRight
                          << ", X Pos: " << payload.xPos
                          << ", Y Pos: " << payload.yPos;
            }
        },
        m_payload);
    std::cout << std::endl;
}

// Serialize the Message object to a string
std::vector<std::byte> Message::serialize() const {
    int payloadLength = sizeof(m_format);
    switch (m_format) {
        case MESSAGE_FORMAT_WHEEL: {
            payloadLength += sizeof(WheelMessage);
            break;
        }
        case MESSAGE_FORMAT_ARM: {
            payloadLength += sizeof(ArmMessage);
            break;
        }
        case MESSAGE_FORMAT_SCIENCE_TOOL: {
            payloadLength += sizeof(ScienceToolMessage);
            break;
        }
        default: { // Generic or unknown
            payloadLength += sizeof(Generic);
            break;
        }
    }
    return std::vector<std::byte>((std::byte*)this, (std::byte*)this + payloadLength);
}

template <typename Payload>
Payload parseMessage(const std::vector<std::byte> data)
{
    if (data.size() - 4 != sizeof(Payload))
    {
        throw std::runtime_error("Serialized MessagePayload size does not match expected." + std::to_string(data.size()) + " " + std::to_string(sizeof(Payload)));
    }

    Payload payload;
    std::memcpy(&payload, data.data() + 4, data.size() - 4);
    return payload;
}

// Deserialize a string to a Message object
Message Message::deserialize(const std::vector<std::byte> data) {
    if (data.size() > sizeof(MessagePayload)) {
        throw std::runtime_error(
            "Payload too large cannot deserialize.");
    }

    if (data.size() < sizeof(int)) {
        throw std::runtime_error("Size of payload too small.");
    }

    MessageFormat format;
    std::memcpy(&format, data.data(), sizeof(format));

    // Deserialize payload based on format
    MessagePayload payload;
    switch (format) {
        case MESSAGE_FORMAT_WHEEL: {
            payload = parseMessage<WheelMessage>(data);
            break;
        }
        case MESSAGE_FORMAT_ARM: {
            payload = parseMessage<ArmMessage>(data);
            break;
        }
        case MESSAGE_FORMAT_SCIENCE_TOOL: {
            payload = parseMessage<ScienceToolMessage>(data);
            break;
        }
        case MESSAGE_FORMAT_GENERIC: { // Generic or unknown
            payload = parseMessage<Generic>(data);
            break;
        }
        default:
            throw std::runtime_error("Unkown data type.");
    }

    Message msg;
    msg.m_format = format;
    msg.m_payload = payload;
    return msg;
}
