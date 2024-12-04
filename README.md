# IoT-Based-Lighting-Control-Using-ESP32-with-MQTT-Communication

This project implements an IoT-based lighting control system using ESP32 microcontrollers, enabling remote management of individual lights, each controlled by a dedicated push button. Each light supports three states: 
- 50% brightness
- 100% brightness
- off

Communication between two ESP32 devices is facilitated using the MQTT protocol, ensuring efficient and reliable data transfer.

**Features**
- Dynamic Brightness Control: Adjust light brightness (50%, 100%, or off) with a single button press.
- Scalable and Modular Design: The system can be extended to control additional lights or devices.
- Efficient Communication: Uses MQTT for fast, reliable, and lightweight data transmission.
- JSON Message Format: Ensures structured and easily parseable data exchange.
- User-Friendly Interface: Physical push buttons for input, with visual feedback through lights.

**System Architecture**



![Project Image](images/led-mqtt-button.png)





The system architecture involves:
- ESP32 Publisher: Reads button states and sends corresponding messages to the MQTT broker.
- MQTT Broker: Handles message delivery between publisher and subscriber.
- ESP32 Subscriber: Controls the brightness of lights based on received messages.

**Hardware Requirements**
- 2 x ESP32 boards
- 3 x Push Buttons
- 3 x LEDs or Light Bulbs (PWM-compatible for brightness control)
- Resistors (as needed for LED and button connections)
- Laptop with MQTT Broker
- Breadboard and Jumper Wires

**Software Implementation**

The system consists of two main parts:
- Publisher (Button Input)
The publisher reads the state of each push button, determines the light's next state (50%, 100%, or off), and sends this information as a JSON message via MQTT.
- Subscriber (Light Control)
The subscriber receives JSON messages from the MQTT broker, parses the data, and adjusts the brightness of the corresponding light using PWM.
