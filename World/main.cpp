#include <iostream>
#include <vector>
#include <string>

// Include GLEW
#ifdef __APPLE__ //For OSX
#define _CRT_SECURE_NO_WARNINGS
#define GL_GLEXT_PROTOTYPES
#include <gl/glew.h>
#endif

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

#include "World.hpp"


/*
 * 用語集
 *
 * vertex attribute
 * : OpenGLにおいて一つの頂点情報は、３Dであるから、３つの要素で成分表示される。この３つの要素の集まり（便宜上ベクトルと呼ぶ）を vertex attribute と呼ぶ
 * : vertex shader はこれ（vertex attribute）を受け取り、これに対して座標系変換、その他変換を行う
 *
 * vertex buffer object
 * : vertex attribute は一つの頂点情報を示すが、この実体は多くの場合、３つのfloatからなる。これはビデオカード上のメモリに保存されている必要があり、頂点情報は複数個からなるため、このまとまりを管理する必要がある。
 * : これら頂点情報群の実体を示し、転送、記録等の管理を行いやすくしまとめているものが、vertex buffer objectである
 *
 * vertex array object
 * : VBO(vertex buffer object)はvertex attributes の実体を保持するが、その実体がどのようにパッキングされVBO中に保存されているかの情報をOpenGLに与える必要がある
 * : その再、glVertexAttribPointer関数を用いて、その設定を行う。これにより、OpenGLは頂点情報を取得できるようになる。しかし、このパッキング情報を描画時に毎回設定するのは効率的でないため、
 * : これらパッキング設定情報を保存するようにする。これがvertex array objectである。
 *
 */


int main(){

    world::Window wd(640, 480, "hello glfw");

//    GLfloat vertices[] = {
//            -0.5f, -0.5f, 0.0f,
//            0.5f, -0.5f, 0.0f,
//            0.0f,  0.5f, 0.0f
//    };

    GLfloat vertices[] = {
            -0.7f, -0.7f, 0.0f,
            0.7f, -0.7f, 0.0f,
            0.0f,  0.7f, 0.0f
    };


    GLfloat vertices_elm[] = {
            0.5f,  0.5f, 0.0f,  // Top Right
            0.5f, -0.5f, 0.0f,  // Bottom Right
            -0.5f, -0.5f, 0.0f,  // Bottom Left
            -0.5f,  0.5f, 0.0f   // Top Left
    };
    GLuint indices_elm[] = {  // Note that we start from 0!
            0, 1, 3,   // First Triangle
            1, 2, 3    // Second Triangle
    };


    //Load Shader
    // Create and compile our GLSL program from the shaders
    const std::string shader_prefix = SHADER_PREFIX; //SHADER_PREFIX is defined by cmake
    const std::string vertex_shader = "t1_vs.glsl";
    const std::string fragment_shader = "t1_fs.glsl";
    GLuint program_id = LoadShaders((shader_prefix + vertex_shader).c_str(), (shader_prefix + fragment_shader).c_str());
    //Use shader program
    glUseProgram(program_id);


    //Generate vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    //Bind VAO
    glBindVertexArray(VAO);

    //Generate vertex buffer including vertex attributes
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Unbind the VAO
    glBindVertexArray(0);


    //Element buffer object version
    GLuint VAO_elm;
    glGenVertexArrays(1, &VAO_elm);
    //Bind
    glBindVertexArray(VAO_elm);
    //普通に、indexed drawされる頂点バッファ情報を転送
    GLuint VBO_elm;
    glGenBuffers(1, &VBO_elm);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_elm);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_elm), vertices_elm, GL_STATIC_DRAW);
    //頂点インデックスを転送 この順番にもとづき、三角形を描画
    GLuint EBO_elm;
    glGenBuffers(1, &EBO_elm);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_elm);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_elm), indices_elm, GL_STATIC_DRAW);
    //VBOの頂点情報のパッキング情報を転送
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0); //0番目のラインを有効化

    glBindVertexArray(0); //VAOのUnbind


//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



    while(wd.IsClose()){
        //Handle events
        wd.HandleEvent();

        /*
         * Render here,
         */
        wd.ClearColor(0.1, 0.6, 0.8); //Clear color buffer

        //Rebind vertex array object
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
        glBindVertexArray(0); //Unbind

        glBindVertexArray(VAO_elm);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //Swap a frame buffer
        wd.SwapBuffers();

    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &VAO_elm);
    glDeleteBuffers(1, &VBO_elm);
    glDeleteBuffers(1, &EBO_elm);


    glDeleteProgram(program_id);

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