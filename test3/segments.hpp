//
// Created by 菊池和気 on 2016/04/28.
//

#ifndef OPENGL_TMP_SEGMENTS_HPP_H
#define OPENGL_TMP_SEGMENTS_HPP_H

/*
 * 自律個セグメントを簡単に扱えるようにする
 * 構成としては、自律個のセグメントを示すセグメントクラスがあり、具体的な問題を実装する場合は、
 * これを継承して、バーチャルな関数を実装する
 *
 * セグメントクラスはセグメントアレイに組み込めれる、自動的に積分計算が行われ、状態量が決定する。
 *
 * 使い方の理想としは
 *
 * ます、セグメントクラスを継承したセグメントを作り、この実体を生成する。
 *
 * #TODO ここ
 *
 * */

/*
 * マツザップ トレーニングメニュー
 * 体幹トレーニング ４０秒 ２０秒の休憩をはさみ３回やる 縦、横それぞれ
 * 腹筋 ２０回で辛くなるなるかんじでやる 大体一周期で８拍くらい
 *
 */

/*
 * 自律系システム全体を表す状態量をどのようにして、表現するか？ということ
 *
 * ここで自律系システム全体を、自律システム系と言うようにする。
 * 自律個の状態を表す状態変数をrとすれば、rは何らかのベクトル量となる。
 * なので、自律系システム系の状態を表す状態量はベクトルrのなす空間Rと考えられる。
 *
 *
 *
 */

#include <vector>

//ここに自律個システムを記述する
/*
 * システムのみが記述され、この実体が状態量を保存しているわけではない
 * */
template <class t>
class Segment{
public:

    //渡すものとしては、系の状態量と、マクロに決定される値
    virtual std::vector<t> Update(const std::vector<t> &r, const std::vector<t> &z); //拘束と、系の状態を入力
    virtual std::vector<t> Update(const std::vector<t> &r); //系の状態のみを入力

    void SetID(const unsigned int id_); //_A, __AAはC++の予約語になっている

private:
    unsigned int id; //シミュレーションマネージャから得られるID

};

#endif //OPENGL_TMP_SEGMENTS_HPP_H
