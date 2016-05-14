#include <iostream>
#include <vector>
#include <string>

// Include GLEW
#ifdef __APPLE__ //For OSX
#include <gl/glew.h>
#define _CRT_SECURE_NO_WARNINGS
#define GL_GLEXT_PROTOTYPES
#endif

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "World.hpp"


int main(){

    world::Window wd(640, 480, "hello glfw");


    while(wd.IsClose()){

        wd.HandleEvent();
        wd.SwapBuffers();


    }

}



//// Include standard headers
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//
//// Include GLEW
//#include <GL/glew.h>
//
//// Include GLFW
//#include <GLFW/glfw3.h>
//GLFWwindow* window;
//
//// Include GLM
//#include <glm/glm.hpp>
//using namespace glm;
//
//#include "shader.hpp"
//
//int main( void )
//{
//    // Initialise GLFW
//    if( !glfwInit() )
//    {
//        fprintf( stderr, "Failed to initialize GLFW\n" );
//        return -1;
//    }
//
//
//    // glfwWindowHint(GLFW_SAMPLES, 4);
//    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    //最大バージョンは4.1だった
//    //glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//
//    // Open a window and create its OpenGL context
//    window = glfwCreateWindow( 1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
//    if( window == NULL ){
//        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLEW
//    glewExperimental = true; // Needed for core profile
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        return -1;
//    }
//
//    // Ensure we can capture the escape key being pressed below
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//    // Dark blue background
//    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//
////    //Enable Depth test
////    glEnable(GL_DEPTH_TEST);
////    //Depth test configuration
////    glDepthFunc(GL_LESS);
//
//
//    //Generate VertexArrayObject
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);
//
//    //Load Shader
//    // Create and compile our GLSL program from the shaders
//    const std::string shader_prefix = SHADER_PREFIX; //SHADER_PREFIX is defined by cmake
//    const std::string vshader = "SimpleVertexShader.vertexshader";
//    const std::string fshader = "SimpleFragmentShader.fragmentshader";
//
//    GLuint programID = LoadShaders((shader_prefix + vshader).c_str(), (shader_prefix + fshader).c_str());
//
//
//    //一回フラグメントシェーダーで書いてみてもいいかもしれない
//    //
//
//    static const GLfloat g_vertex_buffer_data[] = {
//            -1.0f, -1.0f, 0.0f,
//            1.0f, -1.0f, 0.0f,
//            0.0f,  1.0f, 0.0f,
//    };
//
//    GLuint vertexbuffer;
//    //頂点バッファの生成
//    glGenBuffers(1, &vertexbuffer);
//    //バッファのバインド
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    //バッファにデータを転送
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//    do{
//
//        // Clear the screen
//        glClear( GL_COLOR_BUFFER_BIT );
//
//        // Use our shader
//        glUseProgram(programID);
//
//        // 1rst attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(
//                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//                3,                  // size
//                GL_FLOAT,           // type
//                GL_FALSE,           // normalized?
//                0,                  // stride
//                (void*)0            // array buffer offset
//        );
//
//        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//        glDisableVertexAttribArray(0);
//
//        // Swap buffers
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//
//    } // Check if the ESC key was pressed or the window was closed
//    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
//           glfwWindowShouldClose(window) == 0 );
//
//    // Cleanup VBO
//    glDeleteBuffers(1, &vertexbuffer);
//    glDeleteVertexArrays(1, &VertexArrayID);
//    glDeleteProgram(programID);
//
//    // Close OpenGL window and terminate GLFW
//    glfwTerminate();
//
//    return 0;
//}




//急急如律令