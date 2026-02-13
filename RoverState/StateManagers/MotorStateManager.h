#pragma once

#include "ArmMotorStateManager.h"
#include "DriveMotorStateManager.h"
#include "../MotorState.h"

class MotorStateManager {
public:
    MotorStateManager() { }

    MotorStateManager(MotorState initialState) :
        driveStateManager(initialState.driveMotorState),
        armStateManager(initialState.armMotorState) { }

    MotorStateManager& operator=(const MotorStateManager& other) {
        if (this != &other) {
            driveStateManager = other.driveStateManager;
            armStateManager = other.armStateManager;
        }

        return *this;
    }

    void updateDriveMotorState(DriveMotorState newDriveState);

    void updateArmMotorState(ArmMotorState newArmState);

    void updateState(MotorState newState);

    MotorState getState();

    DriveMotorStateManager* getDriveStateManager();
    ArmMotorStateManager* getArmStateManager();

    /** Serializes the MotorState object to a byte array
     *
     * @return
     *  std::vector<std::byte> - The serialized MotorState
     */
    std::vector<std::byte> serialize();

    /** Deserializes a byte array to a MotorState object
     *
     * @param
     *  data: const std::vector<std::byte> - The serialized message byte vector
     *  size: size_t - The size of the message to deserialize - the message will be deserialized from the start
     *  of the byte vector until this number of bytes has been deserialized
     *
     * @return
     *  MotorStateManager - The handler for the deserialized MotorState object
     */
    static MotorState deserialize(const std::vector<std::byte> data, size_t size);

    /** prints contents of MotorState
     *
     * @param
     * none
     *
     * @return
     * none
     */
    void printState();

private:
    DriveMotorStateManager driveStateManager;
    ArmMotorStateManager armStateManager;
};
