//
// Created by 菊池和気 on 2016/05/12.
//

#include <iostream>
#include <vector>
#include <string>

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


using namespace world;


//デフォルトプロファイルでGLFWの初期化を行う
Window::Window()
        : Window(Constants::DEFAULT_WINDOW_WIDTH, Constants::DEFAULT_WINDOW_WIDTH, "Sample")
{
    ;
}

//ウィンドウの名前をつけてウインドウを生成
Window::Window(const std::string & window_name_)
        : Window(Constants::DEFAULT_WINDOW_WIDTH, Constants::DEFAULT_WINDOW_HEIGHT, window_name_)
{
    ;
}

//サイズと名前をつけてウィンドウの生成
Window::Window(unsigned int width_, unsigned int height_, const std::string & window_name_){

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return;
    }

#ifdef __APPLE__
    //最大バージョンは4.1だった @ Yosemite
    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( width_, height_, window_name_.c_str(), nullptr, nullptr);
    if( window == nullptr ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return;
    }

    //このウィンドウを対象に描画を行う
    glfwMakeContextCurrent(window);

#ifdef __APPLE__
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }
#endif

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Register callback function
    glfwSetKeyCallback(window, key_callback);

    //OopeGL view port configuration
    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    glViewport(0, 0, frame_width, frame_height);
}

//デストラクタ
Window::~Window(){
    glfwTerminate();
}

bool Window::IsClose() {
    return !glfwWindowShouldClose(window);
}

void Window::HandleEvent(){
    glfwPollEvents();
}

void Window::SwapBuffers(){
    glfwSwapBuffers(window);
}

const GLFWwindow * Window::GetWindowContext() {
    return window;
}

//イベントハンドラ コールバック関数 これは静的に宣言された関数であるが、これはヘッダファイルでstaticな定義がなされているため、実装部においてはstaticであるとはしてはならない
void Window::key_callback(GLFWwindow* window_, int key_, int scancode_, int action_, int mode_){
    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if(key_ == GLFW_KEY_ESCAPE && action_ == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, GL_TRUE);
}

//画面クリア
void Window::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::ClearColor(double r_, double g_, double b_) {
    glClearColor((float)r_, (float)g_, (float)b_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}












































