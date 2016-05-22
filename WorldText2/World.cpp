//
// Created by 菊池和気 on 2016/05/12.
//

#include <iostream>
#include <vector>
#include <string>


#include <cmath>

// Include GLEW
#ifdef __APPLE__ //For OSX
#include <gl/glew.h>
#define _CRT_SECURE_NO_WARNINGS
#define GL_GLEXT_PROTOTYPES
#endif

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "World.hpp"
#include "shader.hpp"


using namespace std;
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
Window::Window(unsigned int width_, unsigned int height_, const std::string & window_name_)
{

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
    glewExperimental = GL_TRUE; // Needed for core profile
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
//    int frame_width, frame_height;
//    glfwGetFramebufferSize(window, &frame_width, &frame_height);
//    glViewport(0, 0, frame_width, frame_height);

    //Enable Depth test
    glEnable(GL_DEPTH_TEST);
    //Depth test configuration
    glDepthFunc(GL_LESS);

    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /*
     * シェーダロード
     */
    //Load Shader
    // Create and compile our GLSL program from the shaders
    const std::string shader_prefix = SHADER_PREFIX; //SHADER_PREFIX is defined by cmake
    const std::string vertex_shader = "t2_vs.glsl";
    const std::string fragment_shader = "t2_fs.glsl";
    program_id = LoadShaders((shader_prefix + vertex_shader).c_str(), (shader_prefix + fragment_shader).c_str());
    //Use shader program
    glUseProgram(program_id);

    // Load shader with Geometry shader
    const std::string vertex_shader2 = "t3_vs.glsl";
    const std::string geometry_shader2 = "t32_gs.glsl";
    const std::string fragment_shader2 = "t3_fs.glsl";
    geom_program_id = geomLoadShaders((shader_prefix + vertex_shader2), (shader_prefix + fragment_shader2), (shader_prefix + geometry_shader2));
    //Use shader program
//    glUseProgram(geom_program_id);

    // Load shader with Geometry shader
    const std::string vertex_shader3 = "t3_vs.glsl";
    const std::string geometry_shader3 = "t31_gs.glsl";
    const std::string fragment_shader3 = "t3_fs.glsl";
    test_program_id = geomLoadShaders((shader_prefix + vertex_shader3), (shader_prefix + fragment_shader3), (shader_prefix + geometry_shader3));
    //Use shader program
//    glUseProgram(test_program_id);

    // Load shader with Geometry shader
    const std::string vertex_shader_line = "tLine_vs.glsl";
    const std::string geometry_shader_line = "tLine_gs.glsl";
    const std::string fragment_shader_line = "tLine_fs.glsl";
    line_program_id = geomLoadShaders((shader_prefix + vertex_shader_line)
            , (shader_prefix + fragment_shader_line)
            , (shader_prefix + geometry_shader_line)
    );
    //Use shader program
//    glUseProgram(line_program_id);

    // Load shader with Geometry shader
    const std::string vertex_shader_font = "Text_vs.glsl";
    const std::string fragment_shader_font = "Text_fs.glsl";
    font_program_id = LoadShaders((shader_prefix + vertex_shader_font).c_str()
            , (shader_prefix + fragment_shader_font).c_str()
    );
    glUseProgram(font_program_id);


    /*
     * ユニフォームIDの取得
     */
    MVP_id = glGetUniformLocation(program_id, "MVP");
    Color_id = glGetUniformLocation(program_id, "Color");

    geom_MVP_id = glGetUniformLocation(geom_program_id, "MVP");
    geom_Color_id = glGetUniformLocation(geom_program_id, "Color");

    test_MVP_id = glGetUniformLocation(test_program_id, "MVP");
    test_Color_id = glGetUniformLocation(test_program_id, "Color");

    line_MVP_id = glGetUniformLocation(line_program_id, "MVP");
    line_Color_id = glGetUniformLocation(line_program_id, "Color");
    line_thickness_id = glGetUniformLocation(line_program_id, "Thickness"); //線の太さ

    //フォントレンダリング用
    font_P = glGetUniformLocation(font_program_id, "projection;");
    font_TextColor = glGetUniformLocation(font_program_id, "textColor");


    /*
     * 表示中心位置設定
     * View行列の設定
     */
    height_camera = (float)Constants::DEFAULT_CAMERA_HEIGHT;
    SetCenterPoint(glm::vec2(0, 0));


    /*
     * プロジェクション行列の設定
     */
    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    float r = (float)frame_width / (float)frame_height;
    glViewport(0, 0, frame_width, frame_height);
    P = glm::perspective(glm::radians(45.0f), (float)frame_width / (float)frame_height, 0.1f, 100.0f);
//    P_ortho = glm::ortho(-(float)frame_width * 0.5, (float)frame_width * 0.5, -(float)frame_height * 0.5, (float)frame_height * 0.5);
    P_ortho = glm::ortho(-1.0 * r, 1.0 * r, -1.0, 1.0);
    P_text = glm::ortho((float)0.0, (float)frame_width, (float)0.0, (float)frame_height);

}

//デストラクタ
Window::~Window(){

    //シェーダの破棄
    glDeleteProgram(program_id);
    glDeleteProgram(geom_program_id);
    glDeleteProgram(test_program_id);
    glDeleteProgram(line_program_id);

    glfwTerminate();
}

bool Window::IsClose() {
    return !glfwWindowShouldClose(window);
}

void Window::HandleEvent(){
    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    float r = (float)frame_width / (float)frame_height;
    glViewport(0, 0, frame_width, frame_height);
    P = glm::perspective(glm::radians(45.0f), (float)frame_width / (float)frame_height, 0.1f, 100.0f);
//    P_ortho = glm::ortho(-(float)frame_width * 0.5, (float)frame_width * 0.5, -(float)frame_height * 0.5, (float)frame_height * 0.5);
    P_ortho = glm::ortho(-1.0 * r, 1.0 * r, -1.0, 1.0);
    P_text = glm::ortho((float)0.0, (float)frame_width, (float)0.0, (float)frame_height);
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
    ClearColor(0.0, 0.0, 0.0);
}

void Window::ClearColor(double r_, double g_, double b_) {
    glClearColor((float)r_, (float)g_, (float)b_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::ClearColor(const glm::vec3 & color_){
    ClearColor(color_.x, color_.y, color_.z);
}

//中心点の指定 View行列の設定
void Window::SetCenterPoint(double px_, double py_){
    center_position = glm::vec2(px_, py_);

    /*
     * glm::mat4 CameraMatrix = glm::LookAt(
    cameraPosition, // ワールド空間でのカメラの位置
    cameraTarget,   // ワールド空間での見たい位置
    upVector        // たぶんglm::vec3(0,1,0)です。一方で(0,-1,0)にしたら上下逆さまになります。それもまた良いでしょう。
         );
     */
    //Y軸方向が上になるようにする
    V = glm::lookAt(glm::vec3(center_position.x, center_position.y, height_camera)
            , glm::vec3(center_position.x, center_position.y, 0.0)
            , glm::vec3(0, 1, 0));

}
//中心点の指定 上に同様
void Window::SetCenterPoint(const glm::vec2 & p_){
    SetCenterPoint(p_.x, p_.y);
}


/*
 * Graphics Base からVAOを読み、
 * 中心座標、色の設定を行って描画する
 */

void Window::Draw(const GraphicsBase& graphics_base_, double px_, double py_, double r_, double g_, double b_){
    Draw(graphics_base_, glm::vec2(px_, py_), glm::vec3(r_, g_, b_));
}

void Window::Draw(const GraphicsBase& graphics_base_, const glm::vec2 & p_, const glm::vec3 & color_){
    glm::vec3 pos(p_.x, p_.y, 0);
    glm::mat4 M = glm::translate(pos);
    MVP = P * V * M; //MVPの生成

    if(graphics_base_.GetShaderMode() == Constants::DEFAULT_SHADER){
        //シェーダを設定
        glUseProgram(program_id);

        //シェーダにMVPとColorを渡す
        glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(Color_id, 1, &color_[0]);
    }else if(graphics_base_.GetShaderMode() == Constants::DEFAULT_SHADER_LINE){
        //シェーダを設定
        glUseProgram(line_program_id);

        //シェーダにMVPとColorを渡す
        glUniformMatrix4fv(line_MVP_id, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(line_Color_id, 1, &color_[0]);
        //描画の度、DynamicCastなので設計変更？ Dynamic cast ではconst かどうかも見るので注意
        glUniform1f(line_thickness_id, (float)(dynamic_cast<const Line &>(graphics_base_)).GetLineThickness());

    }else if(graphics_base_.GetShaderMode() == Constants::DEFAULT_SHADER_TEST){
        //テストシェーダを設定
        glUseProgram(test_program_id);

        //シェーダにMVPとColorを渡す
        glUniformMatrix4fv(test_MVP_id, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(test_Color_id, 1, &color_[0]);
    }

    //シェーダにMVPとColorを渡す
//    glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);
//    glUniform3fv(Color_id, 1, &color_[0]);

    glBindVertexArray(graphics_base_.GetVaoID());
//    glDrawArrays(GL_LINE_STRIP, 0, graphics_base_.GetNoVertices());
//    glDrawArrays(GL_TRIANGLES, 0, graphics_base_.GetNoVertices());
    glDrawArrays(graphics_base_.GetDrawStyle(), 0, graphics_base_.GetNoVertices());

    glBindVertexArray(0); //Unbind

    return;
}

void Window::Draw(const Line& line_, const glm::vec2 & p_, const glm::vec3 & color_, double thickness_){
    glm::vec3 pos(p_.x, p_.y, 0);
    glm::mat4 M = glm::translate(pos);
//    MVP = P * V * M; //MVPの生成
    MVP = P_ortho * V * M; //MVPの生成

    //シェーダを設定
    glUseProgram(line_program_id);

    //シェーダにMVPとColorを渡す
    glUniformMatrix4fv(line_MVP_id, 1, GL_FALSE, &MVP[0][0]);
    glUniform3fv(line_Color_id, 1, &color_[0]);
    glUniform1f(line_thickness_id, (float)thickness_);

    glBindVertexArray(line_.GetVaoID());
    glDrawArrays(line_.GetDrawStyle(), 0, line_.GetNoVertices());
    glBindVertexArray(0); //Unbind

    return;
}

void Window::Draw(Text & text_
        , const glm::vec2 & p_
        , const glm::vec3 & color_
        , const std::string & txt_
        , double scale_) {

    glUniformMatrix4fv(font_P, 1, GL_FALSE, glm::value_ptr(P_text));
    text_.Render(font_program_id, txt_, p_, static_cast<float>(scale_), color_);


}





































