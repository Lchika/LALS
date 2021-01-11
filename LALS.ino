#include <M5Stack.h>
#include "Targets.hpp"
#include "debug.h"

static void on_init();
static void on_receive_ir(int target_id, bool is_alive);
static void on_not_receive_ir(int target_id, bool is_alive);
static void on_hit(int target_id, int gun_id);

// まとユニット番号、この番号によってIPアドレスが決まるため、他とかぶってはいけない
static constexpr int UNIT_ID = 0;
// このユニットに紐づく赤外線受光モジュールの数
static constexpr int TARGET_NUM = 3;

// Targetsクラスのインスタンスをglobalで定義する
static Targets targets(on_init, on_receive_ir, on_not_receive_ir, on_hit);

void setup()
{
  // M5Stack関係の初期化、ここらへんはお好みで
  M5.begin();
  dacWrite(25, 0);
  M5.Power.begin();

  // まと関係の初期化、M5.begin() or Serial.begin() の後に行う
  targets.begin(UNIT_ID, TARGET_NUM);

  // 赤外線受光モジュールとの疎通確認が可能
  std::vector<int> error_target_ids = targets.get_error_targets();
  for (const auto &id : error_target_ids)
  {
    DebugPrint("<ERROR> failed to connect target[%d]", id);
  }
}

void loop()
{
  // M5Stack関係の更新処理、ボタンを使わないなら多分いらない
  M5.update();
  // まと関係の更新処理、ここでHTTPリクエストの処理をしたり、まとの演出処理をやっている
  targets.update();
}

// ゲーム開始毎の初期化処理、LED消したり、動きを元に戻したりを想定
static void on_init()
{
  // LED消す
  // サーボの位置戻すとか
}

// 赤外線を受光した時の処理
// 引数で対象のまと番号target_id(赤外線受光モジュールのロータリースイッチの値)がとれるので、まと毎に違う処理もできる
// 引数で対象のまとがまだ被弾していないか(is_alive)がとれるので、被弾済みかそうでないかで処理を分けることもできる
// この関数はtargets.update()を呼ばれたタイミングで赤外線を受光していたら実行される
static void on_receive_ir(int target_id, bool is_alive)
{
  // 対象のまとが被弾していない状態だったら
  if (is_alive)
  {
    // 赤外線を受光していることが分かる演出
  }
  else
  {
    // もう被弾済みのまとだったらなにもしないとか
  }
}

// 赤外線を受光していない時の処理
// 引数で対象のまと番号target_id(赤外線受光モジュールのロータリースイッチの値)がとれるので、まと毎に違う処理もできる
// 引数で対象のまとがまだ被弾していないか(is_alive)がとれるので、被弾済みかそうでないかで処理を分けることもできる
// この関数はtargets.update()を呼ばれたタイミングで赤外線を受光していなかったら実行される
static void on_not_receive_ir(int target_id, bool is_alive)
{
  // 対象のまとが被弾していない状態だったら
  if (is_alive)
  {
    // 赤外線を受光していないことが分かる演出
  }
  else
  {
    // もう被弾済みのまとだったらなにもしないとか
  }
}

// 弾が当たった時の処理
// 引数で対象のまと番号target_id(赤外線受光モジュールのロータリースイッチの値)がとれるので、まと毎に違う処理もできる
// 引数で銃番号(gun_id)がとれるので、銃番号ごとに違う処理もできる、銃番号は1始まり
static void on_hit(int target_id, int gun_id)
{
  // LEDを点滅させるとか
  // 銃番号によって処理を変えるとか
  switch (gun_id)
  {
  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
}