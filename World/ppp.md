#2D描画ユーティリティ
##使い方プラン

World wd(640, 480); <-ここでOpenGL、GLFWのイニシャライズ


//できれば
World_window windw1 = wd.CreateWindow(640, 480, 名前);
                        .GenerateWindow(640, 480);

て感じでWorldを覗き見れる窓を作る
どの窓に描画するか（のぞき見た世界を形作るか？）は、

wd.ActivateWindow(windw1);

みたいにする

こいつらは、複雑になるし、アトミック？極限までシンプルな
描画ユーティリティには求められない気がする。
気が向いたら追加する。内部的に使用するだけ。




