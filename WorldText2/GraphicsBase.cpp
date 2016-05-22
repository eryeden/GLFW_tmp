//
// Created by 菊池和気 on 2016/05/21.
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

//Free type
#include <ft2build.h>
#include FT_FREETYPE_H

#include "World.hpp"
#include "shader.hpp"



using namespace std;
using namespace world;


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
//    draw_style = GL_TRIANGLES;
    draw_style = GL_TRIANGLES;

    shader_mode = Constants::DEFAULT_SHADER;

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

// #TODO: 適当なコンストラクタなのでいい感じにする
Line::Line() {
    ;
}


Line::Line(const std::vector<glm::vec2>& points_) {
    SetLineThickness(Constants::DEFAULT_LINE_THICKNESS);
    UploadLine(points_);
}
Line::Line(const std::vector<glm::vec2>& points_, double thickness_) {
    SetLineThickness(thickness_);
    UploadLine(points_);
}

void Line::UploadLine(const std::vector<glm::vec2>& points_) {


    //描画スタイルの設定 これで描画すると最初と最後の計２点が消える　なので、同じ点を最初と最後に追加しておく必要あり
    draw_style = GL_LINE_STRIP_ADJACENCY;
    //最初と最後に同じ点を追加
    //#TODO 変な実装std::deque がおすすめらしい
    points.resize(points_.size() + 2);
    points.front() = points_[0];
    points.insert(++(points.begin()), points_.begin(), points_.end());
    points[points_.size() + 1] = points_.back();
    points.resize(points_.size() + 2);

//    std::cout << "Points_" << std::endl;
//    for(auto pp : points_){
//        std::cout << pp.x << ", " << pp.y << std::endl;
//    }
//
//    std::cout << "Points" << std::endl;
//    for(auto pp : points){
//        std::cout << pp.x << ", " << pp.y << std::endl;
//    }
//
//
//    cout << points_.size() << endl;
//    cout << points.size() << endl;



    no_vertices = (GLuint)points.size(); //頂点数の記録

    //シェーダスタイルの設定
    shader_mode = Constants::DEFAULT_SHADER_LINE;


    //VAO, VBO, EBOの生成
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //普通に、indexed drawされる頂点バッファ情報を転送
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * points.size(), points.data(), GL_STATIC_DRAW);
    //VBOの頂点情報のパッキング情報を転送
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid *)0);
    glEnableVertexAttribArray(0); //0番目のラインを有効化

    glBindVertexArray(0); //VAOのUnbind
}


/*
 * 点描画クラス
 * 実際使うことはないと思う
 * テスト用
 */
Points::Points() {
    ;
}

Points::Points(const std::vector<glm::vec2> & points_) {
    UploadPoints(points_);
}

//vector で受け取った 座標配列をVBOに入れ込む、また、VAOの生成も行う
void Points::UploadPoints(const std::vector<glm::vec2>& points_) {
    //描画する点のかず
    no_vertices = (GLuint)points_.size();

    //描画スタイルの設定
    draw_style = GL_POINTS;

    //シェーダスタイルの設定 とりあえずDEFAULTにしておく
    shader_mode = Constants::DEFAULT_SHADER_TEST;
//    shader_mode = Constants::DEFAULT_SHADER;


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


///*
// * フォントレンダリング用
// */
//class Text: public GraphicsBase{
//public:
//
//    Text();
//    Text(const std::string & path_to_ttf_);
//    Text(const std::string & path_to_ttf_, unsigned int font_loading_size_);
//
//
//
//};

/*
 * コンストラクタでフォントの読み込みまでやってしまう
 */

Text::Text(const std::string & path_to_ttf_, unsigned int font_loading_size_) {

    //Initialize Free Type
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, path_to_ttf_.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, font_loading_size_);

    // Load first 128 characters of ASCII set
    //アスキー文字分のフォントを読み込む
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture 文字のテクスチャをここで生成する
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        //文字とテクスチャIDをマッピングする
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Text::Text(const std::string& path_to_ttf_) {
    Text(path_to_ttf_, Constants::DEFAULT_FONT_LOAD_SIZE);
}

Text::Text() {
    Text(Constants::DEFAULT_FONT_PATH_PREFIX + Constants::DEFAULT_TTF_FILE
            , Constants::DEFAULT_FONT_LOAD_SIZE);
}

void Text::Render(GLuint program_id_
        , const std::string &text_
        , const glm::vec2 &point_
        , GLfloat scale_
        , const glm::vec3 &color_){

    glm::vec2 p = point_;

    // Activate corresponding render state
    glUseProgram(program_id_);
    glUniform3f(glGetUniformLocation(program_id_, "textColor")
            , color_.x, color_.y, color_.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    // Iterate through all characters
//    std::string::const_iterator c;
    for (auto c = text_.begin(); c != text_.end(); c++)
    {
        Character ch = Characters[(*c)];

        GLfloat xpos = p.x + ch.Bearing.x * scale_;
        GLfloat ypos = p.y - (ch.Size.y - ch.Bearing.y) * scale_;

        GLfloat w = ch.Size.x * scale_;
        GLfloat h = ch.Size.y * scale_;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        p.x += (ch.Advance >> 6) * scale_; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
















