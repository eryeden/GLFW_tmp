#include <iostream>
#include <vector>
#include <map>
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
#include <glm/gtx/transform.hpp>

//Free type
#include <ft2build.h>
#include FT_FREETYPE_H

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

    //Initialize Free Type
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    std::string font_dir_prefix = FONTS_DIR_PREFIX; //Defined in CMakeLists.txt
    std::string font_path = "arial.ttf";
    FT_Face face;
    if (FT_New_Face(ft, (font_dir_prefix + font_path).c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    //Load Shader
    //For Text Render
    const std::string shader_prefix = SHADER_PREFIX; //SHADER_PREFIX is defined by cmake
    const std::string vertex_shader_text = "Text_vs.glsl";
    const std::string fragment_shader_text = "Text_fs.glsl";
    GLuint text_program_id = LoadShaders((shader_prefix + vertex_shader_text).c_str(), (shader_prefix + fragment_shader_text).c_str());
    glUseProgram(text_program_id);




    world::Window wd(640, 480, "hello glfw");
    wd.SetCenterPoint(glm::vec2(0.0, 0.0));

    auto no_p = 1000.0;
    auto dt = 0.01;

    std::vector<glm::vec2> pts; pts.resize(no_p);
    for(int i = 0; i < (GLuint)no_p; ++i){
        pts[i].y = ((float)i*dt) * ((float)i*dt);
        pts[i].x = ((float)i*dt);
    }



    std::vector<glm::vec2> ppts;
//    ppts.push_back(glm::vec2(-1, -0.5));
    ppts.push_back(glm::vec2(-1, -0.5));
    ppts.push_back(glm::vec2(0, 0));
    ppts.push_back(glm::vec2(-1, 0.5));
//    ppts.push_back(glm::vec2(-1, 0.5));

    world::Line ln(pts, 0.1);

    //Set points size
    glPointSize(20);
    //Wire frame mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    world::Points opts(ppts);

    world::Circle ccl(0.1, 10);
    world::Circle ccl1(0.1, 10);
    world::Circle ccl2(0.1, 200);

    double i = 0;
    double maxi = 0.05;
    double di = 0.0001;


    while(wd.IsClose()){
        //Handle events
        wd.HandleEvent();

        if(i > maxi) i = 0;
        else i += di;

        /*
         * Render here,
         */
        wd.ClearColor(0.1, 0.6, 0.8); //Clear color buffer

//        wd.Draw(ccl, glm::vec2(0.6, 0.0), glm::vec3(0.1, 0.3, 0.8));
//        wd.Draw(ccl, glm::vec2(0.0, 0.6), glm::vec3(0.5, 0.3, 0.1));
//        wd.Draw(ccl, glm::vec2(0.0, -0.6), glm::vec3(0.1, 0.3, 0.1));
        wd.Draw(ln, glm::vec2(0.0, 0.0), glm::vec3(i * 9, 0.1, i * 9), static_cast<float>(i));
//        wd.Draw(opts, glm::vec2(0.0, 0.0), glm::vec3(0.1, 0.3, 0.1));

        //Swap a frame buffer
        wd.SwapBuffers();

    }


}


/*
 * 急急如律令
 */