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

    namespace Constants{

        const unsigned int DEFAULT_WINDOW_HEIGHT = 640;
        const unsigned int DEFAULT_WINDOW_WIDTH  = 480;
//        const unsigned int DEFAULT_WINDOW_HEIGHT = 640;
//        const unsigned int DEFAULT_WINDOW_HEIGHT = 640;


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
//
//
//        void SetCenterPoint(double px_, double py_); //中心点の指定
//        void SetCenterPoint(const glm::vec2 & p_); //中心点の指定
//        void SetMagnificationRatio(double rate_); //拡大率の指定
//
//
        void Clear(); //画面クリア
        void ClearColor(double r_, double g_, double b_); //画面クリア 色指定
//        void ClearColor(const glm::vec3 & color_); //画面クリア 色指定
//
//
//        void DrawCircle(double r_, double px_, double py_, double r_, double g_, double b_); //丸描画
//        void DrawCircle(const glm::vec3 & p_, const glm::vec3 & color_); //丸描画

        //Event handler, static function must be public
        static void key_callback(GLFWwindow* window_, int key_, int scancode_, int action_, int mode_);


    private:

        //GLFW window context
        GLFWwindow* window; // shared pointer にすべき？




    };

};


#endif //GLFW_TESTS_WORLD_HPP
