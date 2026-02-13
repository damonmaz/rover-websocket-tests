#include "UDPHandler.h"
#include "RoverState/DriveMotorState.h"

int main() {
    MessageQueue queue;

    // Create a message
    DriveMotorState driveToSend;
    driveToSend.drive[0] = 1;
    driveToSend.drive[1] = 2;
    driveToSend.drive[2] = 3;
    driveToSend.drive[3] = 4;

    driveToSend.steer[0] = 5;
    driveToSend.steer[1] = 6;
    driveToSend.steer[2] = 7;
    driveToSend.steer[3] = 8;

    ArmMotorState armToSend;
    armToSend.motorValues[0] = 9;
    armToSend.motorValues[1] = 10;
    armToSend.motorValues[2] = 11;
    armToSend.motorValues[3] = 12;

    MotorState motorStateToSend;
    motorStateToSend.armMotorState = armToSend;
    motorStateToSend.driveMotorState = driveToSend;

    queue.push(motorStateToSend);

    UDPHandler server(8080, 8008);
    server.run(queue);
}
