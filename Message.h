#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once

#include "pub_general.h"
#include <iostream>
#include <stdint.h>
#include <variant>
#include <vector>
#include <sstream>

// Commented out section below allows multiple message types (structs) while
// letting Message have the same format

// Generic message format for default constructor

// Allows for different message formats
using MessagePayload
    = std::variant<Generic, WheelMessage, ArmMessage, ScienceToolMessage>;

class Message {
public:
    /** Constructor for message
     *
     * @param
     *  prty: int - The priority of the message
     *  payload: MessagePayload - The type of struct being put into the message
     *
     * Example Usage:
     *   Message msg1(1, WheelMessage{10, 20, 30});
     *   Message msg2(0, ArmMessage{15, 25});
     */
    Message(MessagePayload payload);
    Message();
    Message(Message const& src);
    Message& operator=(const Message& src);
    ~Message();

    /** prints contents of Message
     *
     * @param
     * none
     *
     * @return
     * none
     */
    void printMessage() const;

    MessageFormat getFormat() const;

    /** Serializes the Message object to a string
     *
     * @return
     *  std::vector<std::byte> - The serialized message
     */
    std::vector<std::byte> serialize() const;

    /** Deserializes a string to a Message object
     *
     * @param
     *  data: const std::vector<std::byte> - The serialized message string
     *
     * @return
     *  Message - The deserialized Message object
     */
    static Message deserialize(const std::vector<std::byte> data, size_t size);

private:
    MessageFormat m_format;
    MessagePayload m_payload; // One of the struct messages
};

#endif