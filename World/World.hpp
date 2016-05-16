//
// Created by 菊池和気 on 2016/05/12.
//

#ifndef GLFW_TESTS_WORLD_HPP
#define GLFW_TESTS_WORLD_HPP

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


namespace world{

    namespace Constants {

        const unsigned int DEFAULT_WINDOW_HEIGHT = 640;
        const unsigned int DEFAULT_WINDOW_WIDTH  = 480;
        const unsigned int DEFAULT_NO_SLICES_CIRCLE = 20;
        const double       DEFAULT_RADIUS_CIRCLE = 1.0;


    };




    /*
    * 描画をサポートする図形としては、円、長方形、直線、曲線、文字
    * 便利機能として、座標軸の描画
    */

    class GraphicsBase{
    public:

        GraphicsBase()
                : vao(0)
                , vbo(0)
                , no_vertices(0)
        {;};

        virtual ~GraphicsBase(){
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        };


        const GLuint GetVaoID() const {return vao;};
        const GLuint GetVboID() const {return vbo;};
        const GLuint GetNoVertices() const {return no_vertices;};

    protected:
        GLuint vao; // vertex array object
        GLuint vbo; // vertex array object
        GLuint no_vertices; //Number of vertices
    };

    /*
     * テストとして、円を書くやつを実装してみる
     * 求める使い方としては、
     *
     * 宣言：Circle ccl(radius, N);
     * ここで、VBO、EBO、VAOの生成を行う
     *
     * 使用： ccl.Draw(point.x, point,y, color);
     * ここで、シェーダに中心座標と、色を転送し、描画する
     * このとき、拡大率を転送して半径を変えられるようにしてもいいかも
     * これら処理はCPUでもできるから、汎用的なシェーダを使える思われる
     *
     * だいたいこんな感じ
     *
     */

    class Circle: public GraphicsBase{
    public:
        //コンストラクタで確実にVAO、VBOを初期化しておく
        Circle();
        Circle(double radius_, unsigned int no_slices_);
        Circle(double radius_);

        //ここで実装すると確実にコードの重複がおきる
        //void Draw(double px_, double py_, double r_, double g_, double b_, const Window & window_);
        //void Draw(glm::vec2 p_, glm::vec3 color_, const Window & window_);

    private:

    };


    class  Window{

    public:

        Window(); //デフォルトのプロファイルでウィンドウを生成
        Window(const std::string & window_name_); //ウィンドウの名前をつけてウインドウを生成
        Window(unsigned int width_, unsigned int height_, const std::string & window_name_); //サイズと名前をつけてウィンドウの生成

        ~Window(); //デストラクタ、とりあえずGLFWコンテクストの破棄だけを行う

        bool IsClose(); // Should close を返す
        void HandleEvent(); //諸々イベントを処理
        void SwapBuffers(); //スワップバッファ

        //いずれ消す
        const GLFWwindow * GetWindowContext();

//        void SetWindowName(const std::string & window_name_); //ウィンドウの名前を設定
//        void SetWindowSize(unsigned int height_, unsigned int width_); //ウィンドウのサイズを設定


        void SetCenterPoint(double px_, double py_); //中心点の指定 View行列の設定
        void SetCenterPoint(const glm::vec2 & p_); //中心点の指定 上に同様
//        void SetMagnificationRatio(double rate_); //拡大率の指定

        void Clear(); //画面クリア
        void ClearColor(double r_, double g_, double b_); //画面クリア 色指定
        void ClearColor(const glm::vec3 & color_); //画面クリア 色指定


//        void DrawCircle(double r_, double px_, double py_, double r_, double g_, double b_); //丸描画
//        void DrawCircle(const glm::vec3 & p_, const glm::vec3 & color_); //丸描画

        /*
         * Graphics Base からVAOを読み、
         * 中心座標、色の設定を行って描画する
         */
        void Draw(const GraphicsBase& graphics_base_, double px_, double py_, double r_, double g_, double b_);
        void Draw(const GraphicsBase& graphics_base_, const glm::vec2 & p_, const glm::vec3 & color_);

        //Event handler, static function must be public
        static void key_callback(GLFWwindow* window_, int key_, int scancode_, int action_, int mode_);


    //private:

        //GLFW window context
        GLFWwindow* window; // shared pointer にすべき？

        //表示中心位置
        glm::vec2 center_position;

        //MVP行列
        glm::mat4 MVP;
        glm::mat4 V;
        glm::mat4 P;

        //カメラ高さ
        GLfloat height_camera;

        //シェーダID
        GLuint program_id;

        GLint MVP_id; //ユニフォームID：MVP
        GLint Color_id; //カラーユニフォームID


    };

};


#endif //GLFW_TESTS_WORLD_HPP
