#include "WebsocketServer.h"

int main() {

    Message genericMsg(0, Generic{42});

    // Wheel message with all fields set
    Message wheelMsg(1, WheelMessage{
        120,   // velocity
        45,    // theta (degrees)
        10     // angle_velocity
    });

    // Arm message with all fields set
    Message armMsg(1, ArmMessage{
        100,   // armXPos
        200,   // armYPos
        300,   // armZPos
        50,    // clawXPos
        60,    // clawYPos
        1,     // clawOpen (1=open, 0=closed)
        90,    // clawRotation (degrees)
        180    // wristRotation (degrees)
    });

    WebSocketServer server(8080);
    server.run(wheelMsg); 
}
