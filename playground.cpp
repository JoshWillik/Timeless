#define WINDOW_WIDTH 640
#define WINDOW_WIDTH 480
#define DEBUG false

//standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <ctime>

//magic graphics file
#include <GL/glew.h>

//mouse and keyboard handler
#include <GL/glfw.h>

//3d math easiness handy stuff. (not needed, makes life easier)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//could use namespace, but I'm not going to
//I want to see what everything belongs to.

//set colour values and prepare them for circling inside void color_change()
float red = 1.0f;
bool redup = true;

float green = 1.0f;
bool greenup = true;

float blue = 1.0f;
bool blueup = true;

float alpha = 1.0f;

void computeMatriciesFromInputs()
{
    //position
    glm::vec3 position = glm::vec3(0,0,5);

    //horizontal angle towards -Z
    float horizontalAngle = 3.14f;
    //^Radians?
    
    //vertical angle 0==horizon
    float verticalAngle = 0.0f;

    //Initial Field of View
    float initialFoV = 45.0f;

    float speed = 3.0f; // units per second

    float mousespeed = 0.005f;

    int x_mouse_pos, y_mouse_pos;

    //get the mouse position
    glfwGetMousePos(&x_mouse_pos, &y_mouse_pos);

    //reset the mouse position
    glfwSetMousePos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    horizontalAngle += mousespeed * STARTHERE


}

GLuint loadBMP_custom(const char *imagepath)
//BMP image texture loading function
{
    //BMPs contain a header which is 54 bytes long
    unsigned char header[54];
    
    //Data starting position finder
    unsigned int dataPos;

    //Image width and height
    unsigned int 
        width, 
        height;

    //The image size, width * height * (blue-byte + redbyte + greenbyte)
    unsigned int imageSize;

    //The actual image data
    unsigned char *data;

    //Actually open the file
    FILE *file = fopen(imagepath, "rb");

    //check for file opening errors
    if(!file)
    {
        printf("Image could not be opened\n");
        return 0;
    }
    
    //read the header and check it for errors
    if (fread(
                header, //pointer needed, target of the read?
                1,      //size of read? 1 byte?
                54,     //amount of reads, based on size?
                file    //file to read from
            ) != 54)     //check for a valid BMP header
    {
        printf("Nto a correct BMP file\n");
        return 0;
    }

    if(header[0] != 'B' || header[1] != 'M') //check for it being a valid header
    {
        printf("Not a correct BMP file\n");
        return 0;
    }

    //Read some information from the byte array
    dataPos     = *(int*)&(header[0x0A]);
    imageSize   = *(int*)&(header[0x22]);
    width       = *(int*)&(header[0x12]);
    height      = *(int*)&(header[0x16]);

    //To correct for misformatted BMP files, guess at some of the information
    if(imageSize == 0)  imageSize=width*height*3;   // amount of pixels times three color values
    if(dataPos == 0)    dataPos=54;                 //Guess at the end of the header

    //create a new buffer
    data = new unsigned char[imageSize];
    
    //read all the image data into the buffer
    fread(
            data,       //read into...
            1,          //read chunk size?
            imageSize,  //amount of data to read
            file        //place to read from
        );

    //close the file
    fclose(file);

    //create an openGL texture object for the data
    GLuint textureID;

    //attach a real object to the above identifier
    glGenTextures(1, &textureID);

    //Bind the new texture as THE 2D texture. All future texture functions will notify
    glBindTexture(GL_TEXTURE_2D, textureID);

    //give the image to openGL
    glTexImage2D(
            GL_TEXTURE_2D,      //The target of this operation
            0,                  //the level, compression maybe?
            GL_RGB,             //the internal format, 3 colors in this case
            width,              //width of the image
            height,             //height of the image
            0,                  //border, documentation says this MUST be 0, why include it?
            GL_BGR,             //format of the color bits: blue green red |didn't work
            GL_UNSIGNED_BYTE,   //the data type of the pixel data
            data                //where the image data is being stored
        );

    //once it's transfered, delete our own version of it
    delete[] data;

    //two unexplained lines, checking documentation..
    //seems to be filtering something by distance
    glTexParameteri(
            GL_TEXTURE_2D,          //target
            GL_TEXTURE_MAG_FILTER,  //limit, in this case, specifying that the rule applies to what happens when the rendering gets too big for the texture
            //GL_NEAREST              //just take the closest pixel to the float value, low quality
            GL_LINEAR               //find a goodish color by sampling the nearest pixels, not quite as good as ansiotrphic, but faster
        );
    glTexParameteri(
            GL_TEXTURE_2D,          //target
            GL_TEXTURE_MIN_FILTER,  //specifying what happens when the texture is too big for the object
            //GL_NEAREST              //just take the closest pixel to the float value, low quality
            GL_LINEAR               //find a goodish color by sampling the nearest pixels, not quite as good as ansiotrphic, but faster
        );
    glGenerateMipmap(GL_TEXTURE_2D); //generate mini-images for smaller objects. Time saver I guess

    return textureID;
}
void cube_color_change(GLfloat *array)
{
    int i = 0;
    int num_colors = 3 * 3 * 12; //number of verticies to morph
    GLfloat tempfloat; //holds float temporarily while logic is performed

    for(i = 0; i < num_colors; i++) //for the range of verticies
    {
        tempfloat = (GLfloat)rand()/(GLfloat)RAND_MAX; // make a temp random float
        tempfloat /= 50.0f; //tone the change down a bit, make it slower
        if(array[i] >= 1.0f) array[i] -= tempfloat; //if the color is already at or above 1.0f, subtract
        else if(array[i] <= 0.0f) array[i] += tempfloat; //if at or below 0, add
        else{                                           //otherwise
            if(rand() % 2) array[i] += tempfloat;   //generate random number to decide to add or subract
            else array[i] -= tempfloat;
        }

    }
}

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

void draw_triangle(GLuint buffer, GLuint colors)
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

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors);
    glVertexAttribPointer(
        1,           //1, must match layout in shader
        2,           // size, 3 for color, 2 for texture. TODO figure out Why?
        GL_FLOAT,    //type
        GL_FALSE,    //normalized?
        0,           //stride, whatever that is
        (void*)0     //array buffer offset
    );

    // Draw the triangle!
    glDrawArrays(GL_TRIANGLES, 0, 3 * 12); //starting at 0 and going to three, order matters I guess?

    glDisableVertexAttribArray(0);

    if(DEBUG)
        sleep(1);
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
    //initialize random number
    srand((unsigned)time(0));

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


    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    int redbits = 0;
    int greenbits = 0;
    int bluebits = 0;
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

    //Check for depth, so things don't render on top of eachother
    glEnable(GL_DEPTH_TEST);
    //Draw closer things last
    glDepthFunc(GL_LEQUAL);

    //NOTE 1
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    //makin' shaders happen
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    //get the location of the shader ID
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        float
        up = 1.0f,
        down = -1.0f,
        right = 1.0f,
        left  = -1.0f,
        forward = -1.0f,
        back = 1.0f
        ;

    static const GLfloat g_vertex_buffer_data[] = {
        right, up, forward,
        left, up, forward,
        left, down, forward,
        //back top triangle

        right, up, forward,
        right, down, forward,
        left, down, forward,
        //back bottom triangle
        
        right, up, back,
        left, up, back,
        left, down, back,
        //top front triangle

        right, up, back,
        right, down, back,
        left, down, back,
        //bottom front triangle

        left, down, forward,
        left, down, back,
        right, down, back,
        //forward bottom triangle

        left, down, forward,
        right, down, forward,
        right, down, back,
        //back bottom triangle

        left, up, back,
        left, up, forward,
        left, down, forward,
        //left top triangle

        left, down, back,
        left, down, forward,
        left, up, back,
        //left bottom triangle

        right, up, back,
        right, up, forward,
        right, down, forward,
        //right top triangle

        right, down, back,
        right, down, forward,
        right, up, back,
        //right bottom triangle
        
        left, up, forward,
        left, up, back,
        right, up, back,
        //forward bottom triangle

        left, up, forward,
        right, up, forward,
        right, up, back,
        //back bottom triangle

    };

    GLfloat 
        uvone = 1.0f,
        uvnone= 0.0f;

    static const GLfloat g_uv_buffer_data[] = {
        uvone, uvone,
        uvnone, uvone,
        uvnone, uvnone,

        uvone, uvone,
        uvone, uvnone,
        uvnone, uvnone,

        uvone, uvone,
        uvnone, uvone,
        uvnone, uvnone,

        uvone, uvone,
        uvone, uvnone,
        uvnone, uvnone,

        uvnone, uvone,
        uvnone, uvnone,
        uvone, uvnone,
        
        uvnone, uvone,
        uvone, uvone,
        uvone, uvnone,

        uvone, uvnone,
        uvone, uvone,
        uvnone, uvone,

        uvnone, uvnone,
        uvnone, uvone,
        uvone, uvnone,

        uvone, uvnone,
        uvone, uvone,
        uvnone, uvone,

        uvnone, uvnone,
        uvnone, uvone,
        uvone, uvnone,

        uvnone, uvone,
        uvnone, uvnone, 
        uvone, uvnone,

        uvnone, uvone,
        uvone, uvone,
        uvone, uvnone,
    };

    static GLfloat g_color_buffer_data[3*3*12];
    cube_color_change(g_color_buffer_data);

    //identify a vertex buffer
    GLuint vertexbuffer;
    
    //Generate a buffer and put it in the address of the one we just declared
    glGenBuffers(1, &vertexbuffer);

    //bind the buffer to the vertecies
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    //give openGL the verticies
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint texturebuffer;
    glGenBuffers(1, &texturebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof( g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    //Get texture
    GLuint Texture = loadBMP_custom("disco.bmp");

    while (running)
    {
        //set the color of screen background. The skybox, if you will.
        //NOTE 2: can this be made into a picture instead? Research
        glClearColor(red, green, blue, alpha);
        
        //clear the screen?
        glClear(GL_COLOR_BUFFER_BIT); 

        //testing rendering
        //if(DEBUG) sleep(1);

        //using our shader
        glUseProgram(programID);

        //set perspective for 3d space
        glm::mat4 Projection = glm::perspective(
            45.0f, // FOV in degrees
            4.0f / 3.0f, // aspect ratio, 4:3
            0.1f, //minimum cutoff render distance
            100.0f //maximum cutoff render distance
            );

        //create the camera matrix
        glm::mat4 View = glm::lookAt(
                            glm::vec3(4,3,3), //the exact point the camera is located at in worldspace
                            glm::vec3(0,0,0), //the point the camera is looking at
                            glm::vec3(0,1,0)  //orientation of the camera, y=1 for upright, y=-1 for upsidedown
                        );

        //identify the model matrix (at the origin)
        glm::mat4 Model = glm::mat4(1.0f); //unsure of what the argument does

        //applying the transformations to the MVP location of the shader
        glm::mat4 MVP = Projection * View * Model; //right to left multiplication takes place
            
        //send the transforms to the shader
        //in the "MVP" container?
        glUniformMatrix4fv(
                MatrixID,   //the location of the transforms
                1,          //the count of transforms?
                GL_FALSE,   //transpose? Whatever that means
                &MVP[0][0]  //a float, for some reason
            );
        
        draw_triangle(vertexbuffer, texturebuffer);
        color_change();
        
        //debug info
        if(DEBUG){
            std::cout << "Red " << red << " | Green " << green << " | Blue " << blue << std::endl;
        }
        
        //swap front and back buffers (new renders are done in the background?)
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


///////////////////////
//NOTES
///////////////////////
//
//NOTE 1: What do these functions do? Why are they there? Discover purpose/usefullness of these
//
//NOTE 2: can this be made into a picture instead? Research
