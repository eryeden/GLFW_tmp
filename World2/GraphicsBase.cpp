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









































