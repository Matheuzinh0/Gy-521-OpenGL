#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <libserial/SerialStream.h>
#include <string>
#include <sstream>
#include <cmath>

using namespace LibSerial;

float pitch = 0.0f, roll = 0.0f;

void updateAngles(SerialStream &serialStream) {
    if (serialStream.good()) {
        std::string line;
        std::getline(serialStream, line);
        std::stringstream ss(line);
        std::string pitchStr, rollStr;

        std::getline(ss, pitchStr, ',');
        std::getline(ss, rollStr);

        pitch = std::stof(pitchStr);
        roll = std::stof(rollStr);
    }
}

void drawAirplane() {
    glPushMatrix();
    glRotatef(pitch, 1.0f, 0.0f, 0.0f); // Pitch
    glRotatef(roll, 0.0f, 0.0f, 1.0f); // Roll

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glPopMatrix();
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Avião OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    SerialStream serialStream("/dev/ttyUSB0");
    
    // Set baud rate
 //   serialStream.SetBaudRate(SerialStreamBuf::BAUD_115200);  // Replace this if necessary
    serialStream.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
    serialStream.SetFlowControl(FlowControl::FLOW_CONTROL_NONE);

    // If the above line fails, try this instead:
    // serialStream.SetBaudRate(115200);  // Using integer value directly

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        updateAngles(serialStream);
        drawAirplane();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    serialStream.Close();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

