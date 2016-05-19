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
    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    glViewport(0, 0, frame_width, frame_height);

    //Enable Depth test
    glEnable(GL_DEPTH_TEST);
    //Depth test configuration
    glDepthFunc(GL_LESS);

    /*
     * シェーダロード
     */
    //Load Shader
    // Create and compile our GLSL program from the shaders
    const std::string shader_prefix = SHADER_PREFIX; //SHADER_PREFIX is defined by cmake
    const std::string vertex_shader = "t2_vs.glsl";
    const std::string fragment_shader = "t2_fs.glsl";
    GLuint program_id = LoadShaders((shader_prefix + vertex_shader).c_str(), (shader_prefix + fragment_shader).c_str());
    //Use shader program
    glUseProgram(program_id);

    /*
     * ユニフォームIDの取得
     */
    MVP_id = glGetUniformLocation(program_id, "MVP");
    Color_id = glGetUniformLocation(program_id, "Color");

    /*
     * 表示中心位置設定
     * View行列の設定
     */
    height_camera = (float)Constants::DEFAULT_CAMERA_HEIGHT;
    SetCenterPoint(glm::vec2(0, 0));

    /*
     * プロジェクション行列の設定
     * 射影行列：45&deg;の視界、アスペクト比4:3、表示範囲：0.1単位  100単位
     */
    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


}

//デストラクタ
Window::~Window(){

    //シェーダの破棄
    glDeleteProgram(program_id);
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
void Window::Draw(const GraphicsBase& graphics_base_, const glm::vec2 & p_, const glm::vec3 & color_){
    glm::vec3 pos(p_.x, p_.y, 0);
    glm::mat4 M = glm::translate(pos);
    MVP = P * V * M; //MVPの生成

    //シェーダにMVPとColorを渡す
    glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);
    glUniform3fv(Color_id, 1, &color_[0]);

    glBindVertexArray(graphics_base_.GetVaoID());
//    glDrawArrays(GL_LINE_STRIP, 0, graphics_base_.GetNoVertices());
//    glDrawArrays(GL_TRIANGLES, 0, graphics_base_.GetNoVertices());
    glDrawArrays(graphics_base_.GetDrawStyle(), 0, graphics_base_.GetNoVertices());

    glBindVertexArray(0); //Unbind

    return;
}
void Window::Draw(const GraphicsBase& graphics_base_, double px_, double py_, double r_, double g_, double b_){
    Draw(graphics_base_, glm::vec2(px_, py_), glm::vec3(r_, g_, b_));
}


//円描画
Circle::Circle()
        : Circle(Constants::DEFAULT_RADIUS_CIRCLE, Constants::DEFAULT_NO_SLICES_CIRCLE)
{
    ;
}

Circle::Circle(double radius_)
        : Circle(radius_, Constants::DEFAULT_NO_SLICES_CIRCLE)
{
    ;
}


/*
 * コンストラクタにおいて、円の頂点群を生成し、このときのVAOを保存しておく
 */
Circle::Circle(double radius_, unsigned int no_slices_){

    //頂点数の設定
    no_vertices = no_slices_;

    //描画スタイルの設定
    draw_style = GL_TRIANGLES;

    //円の頂点群の生成
    vector<glm::vec2> vtx; vtx.resize(no_slices_);
    double dtheta = 2.0 * M_PI / (double)no_slices_;

    //三角形群の生成
    vector<glm::vec2> vtxs;
    for(unsigned int i = 0; i < no_slices_; ++i){

        vtxs.push_back(glm::vec2(
                (float)(radius_ * cos(dtheta * (double)i))
                , (float)(radius_ * sin(dtheta * (double)i))
        ));
        vtxs.push_back(glm::vec2(0,0));
        vtxs.push_back(glm::vec2(
                (float)(radius_ * cos(dtheta * (double)(i+1)))
                , (float)(radius_ * sin(dtheta * (double)(i+1)))
        ));
    }
    no_vertices = (GLuint)vtxs.size();

    //VAO, VBO, EBOの生成
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //普通に、indexed drawされる頂点バッファ情報を転送
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vtxs.size(), vtxs.data(), GL_STATIC_DRAW);
    //VBOの頂点情報のパッキング情報を転送
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid *)0);
    glEnableVertexAttribArray(0); //0番目のラインを有効化

    glBindVertexArray(0); //VAOのUnbind
}



///*
// * 曲線描画クラス
// * とりあえず、線の太さ忘れて、１ドットの線だけを書くことを考える。
// * コンストラクタで直線をなす点群の座標を与えるとする
// */
//class Line:GraphicsBase{
//public:
//    Line();
//    Line(const std::vector<glm::vec2> & points_);
//
//    void UploadLine(const std::vector<glm::vec2> & points_);
//
//private:
//    std::vector<glm::vec2> points;
//};


Line::Line() {

}

Line::Line(const std::vector<glm::vec2>& points_) {
    UploadLine(points_);
}

void Line::UploadLine(const std::vector<glm::vec2>& points_) {
    no_vertices = (GLuint)points_.size();
    //描画スタイルの設定
    draw_style = GL_LINE_STRIP;
    //VAO, VBO, EBOの生成
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //普通に、indexed drawされる頂点バッファ情報を転送
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * points_.size(), points_.data(), GL_STATIC_DRAW);
    //VBOの頂点情報のパッキング情報を転送
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid *)0);
    glEnableVertexAttribArray(0); //0番目のラインを有効化

    glBindVertexArray(0); //VAOのUnbind
}








































