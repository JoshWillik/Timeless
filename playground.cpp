#define DEBUG true

//standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

//magic graphics file
#include <GL/glew.h>

//mouse and keyboard handler
#include <GL/glfw.h>

//3d math easiness handy stuff. (not needed, makes life easier)
#include <glm/glm.hpp>
//could use namespace, but I'm not going to
//I want to see what everything belongs to.

int main()
{
    int running = GL_TRUE;

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    int width = 640;
    int height = 480;
    int redbits = 5;
    int greenbits = 6;
    int bluebits = 5;
    int alphabits = 0;
    int depthbits = 8;
    int stencilbits = 0;
    //GLFW_WINDOW or GLFW_FULLSCREEN

    if(glfwOpenWindow(width, height, redbits, greenbits, bluebits, alphabits, depthbits, stencilbits, GLFW_WINDOW) != GL_TRUE)
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    glfwSetWindowTitle("Timeless");

    float red = 1.0f;
    bool redup = true;

    float green = 1.0f;
    bool greenup = true;

    float blue = 1.0f;
    bool blueup = true;

    float alpha = 1.0f;

    while (running)
    {
        //render stuff
        glClearColor(red, green, blue, alpha);
        glClear( GL_COLOR_BUFFER_BIT); 

        //change up background colour
        if((red > 1.0f && redup) || (red < 0.0f && !redup))
        {
              if(redup) redup = false;
              else redup = true;
        }
        if(redup) red+= 0.01f;
        else red-= 0.01f;

        if((green > 1.0f && greenup) || (green < 0.0f && !greenup))
        {
              if(greenup) greenup = false;
              else greenup = true;
        }
        if(greenup) green+= 0.03f;
        else green-= 0.03f;
        
        if((blue > 1.0f && blueup) || (blue < 0.0f && !blueup))
        {
              if(blueup) blueup = false;
              else blueup = true;
        }
        if(blueup) blue+= 0.05f;
        else blue-= 0.05f;

        //debug info
        if(DEBUG){
            std::cout << "Red " << red << " | Green " << green << " | Blue " << blue << std::endl;
        }
        //swap front and back buffers (new renders are done in the background?
        glfwSwapBuffers();

        //check for end conditions
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam (GLFW_OPENED);

    }

    return 0;
}
