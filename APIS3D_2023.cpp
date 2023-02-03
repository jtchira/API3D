// APIS3D_2023.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "common.h"
#include <math.h>
#include <map>
#include "Vertex.h"


class EventState {
public:
    std::map<int, bool> keyState;
    int posX, posY;
    int oldPosX, oldPosY;

    EventState()
    {
        for (int i = 0; i < GLFW_KEY_LAST; i++)
        {
            keyState[i] = false;
        }
        posX = posY = oldPosX = oldPosY = 0;
    }

};

EventState events;

class polygon {
public:
    std::vector<Vertex> vertices;
    glm::vec4 color;
    glm::vec4 position;
    glm::vec4 rotation;
    glm::vec4 scaling;

    glm::mat4 modelMtx;
    float velocity;
    polygon()
    {
        vertices.resize(3);
        Vertex vertex1;
        vertex1.vertex_t.pos = glm::vec4(-0.5f,-0.5f, 0.0f, 1.0f);
        vertex1.vertex_t.color = glm::vec4(1.0, 0.0f, 0.0f, 1.0f);

        Vertex vertex2;
        vertex2.vertex_t.pos = glm::vec4( 0.0f, 0.5f, 0.0f, 1.0f);
        vertex2.vertex_t.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        Vertex vertex3;
        vertex3.vertex_t.pos = glm::vec4( 0.5f,-0.5f, 0.0f, 1.0f);
        vertex3.vertex_t.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

        vertices[0] = vertex1;
        vertices[1] = vertex2;
        vertices[2] = vertex3;
        
        
        

        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        velocity = 0.4f;
        position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        scaling  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        modelMtx = glm::mat4(1.0f);
    }

    void update(float deltaTime)
    {
        if(events.keyState[GLFW_KEY_D])
            position.x += deltaTime * velocity;


        if (events.keyState[GLFW_KEY_A])
            position.x -= deltaTime * velocity;

        if (position.x > 1.5f)
            position.x = -1.5f;
        if (position.x < -1.5f)
            position.x = 1.5f;
    }


    void updateModelMatrix()
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position));
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scaling));
        this->modelMtx = model;
    }
};



class timeManager {
public:
    float newTime = 0;
    float deltaTime = 0;
    float lastTime = 0;

    void update() {
        newTime = static_cast<float>(glfwGetTime());
        deltaTime = newTime - lastTime;
        lastTime = newTime;

    }


};


void keyManager(GLFWwindow* window, int key, int scancode, int action, int
    mods)
{

    switch (action)
    {
    case GLFW_PRESS:

        std::cout << "Tecla apretada\n";
        events.keyState[key] = true;
        break;
    case GLFW_RELEASE:
        std::cout << "Tecla levantada\n";
        events.keyState[key] = false;
        break;
    
    }
}

void mouseManager(GLFWwindow* window, double xpos, double ypos) {

    events.oldPosX = events.posX;
    events.oldPosY = events.posY;
    events.posX = xpos;
    events.posY = ypos;
    std::cout<<"Posición " << xpos << " " << ypos << "\n";

}

void mouseKeyManager(GLFWwindow* window, int button, int action, int mods) {
    
    switch (action)
    {
    case GLFW_PRESS:

        std::cout << "Boton ratón apretado\n";
        events.keyState[button] = true;
        break;
    case GLFW_RELEASE:
        std::cout << "Boton ratón levantado\n";
        events.keyState[button] = false;
        break;

    }
}


void drawPolygon(polygon &p)
{

    p.updateModelMatrix();
    glBegin(GL_TRIANGLES);
    glColor3f(p.color.r, p.color.g, p.color.b);
    for (auto vert : p.vertices)
    {
        vert.vertex_t.pos = p.modelMtx * vert.vertex_t.pos;
        glColor3f(vert.vertex_t.color.r, vert.vertex_t.color.g, vert.vertex_t.color.b);
        glVertex3f(vert.vertex_t.pos.x, vert.vertex_t.pos.y, vert.vertex_t.pos.z);
    }
    glEnd();
    /*
    glPushMatrix();
    glMultMatrixf(&(p.modelMtx[0][0]));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, sizeof(vertex_t), p.vertices.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(vertex_t), &(p.vertices.data()->color));
    glDrawElements(GL_TRIANGLES, p.indexes.size(), GL_UNSIGNED_INT, p.indexes.data());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
    */
}


int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        std::cout << "ERROR GLFWINIT\n";
    }


    GLFWwindow* window= glfwCreateWindow(1920, 1080, "3D API Render", 
        nullptr, nullptr);

    //Create gpu context
    glfwMakeContextCurrent(window);

    //Init events managers
    glfwSetKeyCallback(window, keyManager);
    glfwSetCursorPosCallback(window, mouseManager);
    glfwSetMouseButtonCallback(window, mouseKeyManager);
    
    gladLoadGL(glfwGetProcAddress);

    timeManager t;
    polygon p;
    while (!glfwWindowShouldClose(window))
    {

        //limpiar buffer color
        glClear(GL_COLOR_BUFFER_BIT);
        //actualizar

        t.update();
        p.update(t.deltaTime);
        //dibujar
        drawPolygon(p);

        //Interchange front and back buffers
        glfwSwapBuffers(window);

        //execute events
        glfwPollEvents();
    }

    std::cout << "MAIN EXIT\n";
    glfwTerminate();
    return 0;
}
