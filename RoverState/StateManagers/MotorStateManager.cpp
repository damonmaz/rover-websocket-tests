#include "MotorStateManager.h"
#include <cstring>

void MotorStateManager::updateDriveMotorState(DriveMotorState newDriveState) {
    driveStateManager.updateState(newDriveState);
}

void MotorStateManager::updateArmMotorState(ArmMotorState newArmState) {
    armStateManager.updateState(newArmState);
}

void MotorStateManager::updateState(MotorState newState) {
    driveStateManager.updateState(newState.driveMotorState);
    armStateManager.updateState(newState.armMotorState);
}

MotorState MotorStateManager::getState() {
    MotorState outputState = { .driveMotorState = driveStateManager.getState(),
                               .armMotorState = armStateManager.getState() };

    return outputState;
}

DriveMotorStateManager* MotorStateManager::getDriveStateManager() {
    return &driveStateManager;
}

ArmMotorStateManager* MotorStateManager::getArmStateManager() {
    return &armStateManager;
}

void MotorStateManager::printState() {
    std::cout << "Drive - steer: " << getState().driveMotorState.steer[0]
                << " " << getState().driveMotorState.steer[1]
                << " " << getState().driveMotorState.steer[2]
                << " " << getState().driveMotorState.steer[3] // TODO: hardcoded for 4 wheels, update later
                << std::endl;
    std::cout << "Drive - drive: " << getState().driveMotorState.drive[0]
                << " " << getState().driveMotorState.drive[1]
                << " " << getState().driveMotorState.drive[2]
                << " " << getState().driveMotorState.drive[3]
                << std::endl;
    std::cout << "Arm: " << getState().armMotorState.motorValues[0]
                << " " << getState().armMotorState.motorValues[1]
                << " " << getState().armMotorState.motorValues[2]
                << " " << getState().armMotorState.motorValues[3]
                << std::endl;
}

// Serialize the MotorStateManager object to a byte vector for transmission
std::vector<std::byte> MotorStateManager::serialize() {
    int payloadLength = sizeof(MotorState);
    MotorState stateToSend = this->getState();

    return std::vector<std::byte>((std::byte*)&stateToSend, (std::byte*)&stateToSend + payloadLength);
}

// Deserialize the received byte array to a MotorStateManager object
MotorState MotorStateManager::deserialize(const std::vector<std::byte> data, size_t size) {

    // Deserialize payload
    if (size != sizeof(MotorState))
    {
        throw std::runtime_error("Serialized MessagePayload size does not match expected. Expected: " + std::to_string(sizeof(MotorStateManager)) + " Received: " + std::to_string(sizeof(data.size())));
    }

    MotorState payload;
    std::memcpy(&payload, data.data(), size);

    return payload;
}

