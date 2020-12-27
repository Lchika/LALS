#include <M5Stack.h>
#include "Targets.hpp"

static void on_init();
static void on_receive_ir(int target_id);
static void on_not_receive_ir(int target_id);
static void on_hit(int target_id);

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
  dacWrite(25,0);
  M5.Power.begin();

  // まと関係の初期化、M5.begin() or Serial.begin() の後に行う
  targets.begin(UNIT_ID, TARGET_NUM);

  // 赤外線受光モジュールとの疎通確認が可能
  std::vector<int> error_target_ids = targets.get_error_targets();
  for (const auto &id : error_target_ids)
  {
    Serial.printf("<ERROR> failed to connect target[%d]", id);
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
}

// 赤外線を受光した時の処理、引数で対象のまと番号(赤外線受光モジュールのロータリースイッチの値)がとれるので、まと毎に違う処理もできる
// この関数はtargets.update()を呼ばれたタイミングで赤外線を受光していたら実行される
static void on_receive_ir(int target_id)
{
}

// 赤外線を受光していない時の処理、引数で対象のまと番号(赤外線受光モジュールのロータリースイッチの値)がとれるので、まと毎に違う処理もできる
// この関数はtargets.update()を呼ばれたタイミングで赤外線を受光していなかったら実行される
static void on_not_receive_ir(int target_id)
{
}

// 弾が当たった時の処理、引数で対象のまと番号(赤外線受光モジュールのロータリースイッチの値)がとれるので、まと毎に違う処理もできる
static void on_hit(int target_id)
{
}