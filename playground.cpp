#define DEBUG false

//standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

//magic graphics file
#include <GL/glew.h>

//mouse and keyboard handler
#include <GL/glfw.h>

//3d math easiness handy stuff. (not needed, makes life easier)
#include <glm/glm.hpp>
//could use namespace, but I'm not going to
//I want to see what everything belongs to.

//set colour values and prepare them for circling
float red = 1.0f;
bool redup = true;

float green = 1.0f;
bool greenup = true;

float blue = 1.0f;
bool blueup = true;

float alpha = 1.0f;


void color_change()
{
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
}

void draw_triangle(GLuint buffer)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(
        0,           //0, no reason?
        3,           // size
        GL_FLOAT,    //type
        GL_FALSE,    //normalized?
        0,           //stride, whatever that is
        (void*)0     //array buffer offset
    );

    // Draw the triangle!
    glDrawArrays(GL_TRIANGLES, 0, 3); //starting at 0 and going to three, order matters I guess?

    glDisableVertexAttribArray(0);
}

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
{
    //creating shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    //Read the shader code from a file
    std::string VertexShaderCode;
    //ifstream == in-file-stream?
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }else{
        printf("ERROR");
        return 0;
    }

    //Read the shader code from a file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    //compile the Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);    

    //check the above
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    //compile the Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);    

    //check the above
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    
    //linking the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    //Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( glm::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    //delete the temp shaders I guess
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

int main()
{
    int running = GL_TRUE;

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    int width = 640;
    int height = 480;
    int redbits = 5;
    int greenbits = 6;
    int bluebits = 5;
    int alphabits = 0;
    int depthbits = 32;
    int stencilbits = 0;
    //GLFW_WINDOW or GLFW_FULLSCREEN

    if(glfwOpenWindow(width, height, redbits, greenbits, bluebits, alphabits, depthbits, stencilbits, GLFW_WINDOW) != GL_TRUE)
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    glfwSetWindowTitle("Timeless");

    //next line to stop glGenVertexArrays from segfaulting
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    //makin' shaders happen
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    //identify a vertex buffer
    GLuint vertexbuffer;
    
    //Generate a buffer and put it in the address of the one we just declared
    glGenBuffers(1, &vertexbuffer);

    //bind the buffer to the vertecies
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    //give openGL the verticies
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    while (running)
    {
        //render stuff
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT); 
        glUseProgram(programID);

        draw_triangle(vertexbuffer);
        color_change();

        //debug info
        if(DEBUG){
            std::cout << "Red " << red << " | Green " << green << " | Blue " << blue << std::endl;
        }
        
        //swap front and back buffers (new renders are done in the background?
        glfwSwapBuffers();

        //check for end conditions
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam (GLFW_OPENED);
    }

    // Close window and terminate everything nicely
    glfwTerminate();

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}
