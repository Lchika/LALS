#ifndef TARGETS_HPP
#define TARGETS_HPP

#include "Target.hpp"
#include "TargetServer.hpp"

class Targets
{
public:
  /**
   * @brief コンストラクタ
   * @param on_init ゲーム開始時に毎回行う初期化処理
   * @param on_receive_ir 赤外線を受信した時の演出処理
   * @param on_not_receive_ir 赤外線を受信していない時に演出処理
   * @param on_hit 弾が当たった時の演出処理
   */
  Targets(void (*on_init)(),
          void (*on_receive_ir)(int, bool),
          void (*on_not_receive_ir)(int, bool),
          void (*on_hit)(int, int));
  /**
   * @brief 最初に1度だけ行う必要がある初期化処理
   * @param unit_id まとユニットの番号、0始まりで指定する
   * @param targets_num まとの個数
   * @param begin_wifi true:WiFi.begin()を実行する, false:WiFi.begin()を実行しない
   * @return bool true:成功, false:失敗
   * @attention Serial.begin() or M5.begin() 後に呼び出す必要がある。
   * 
   * 初期化処理は本当はコンストラクタでまとめてやってもよいのだが、
   * 一部エラーをシリアル出力したい処理があるのでそれはこっちでやる。
   */
  bool begin(int unit_id, int targets_num, bool begin_wifi = true);
  /**
   * @brief 異常状態になっている的を取得する
   * @return std::vector<int> 異常状態のまとのid、無ければ空のvectorを返す。
   */
  std::vector<int> get_error_targets(void);
  /**
   * @brief 更新処理
   * 
   * この処理の中で、条件を満たせば赤外線受信演出を行う。
   * この処理はM5.update()のように定期的に呼び出す必要がある。
   */
  void update();
  static int alive_target_num;

private:
  std::unique_ptr<TargetServer> _server;
  static std::vector<Target> _targets;
  void (*_on_receive_ir)(int, bool);
  void (*_on_not_receive_ir)(int, bool);
  static void (*_on_init)(void);
  static void (*_on_hit)(int, int);
  
  static void _handle_shoot(WebServer *server);
  static void _handle_init(WebServer *server);
  static void _response_to_center(WebServer &server, int response_num);
  static bool _is_hit(const Target &target, int shoot_gun_num_i);
  bool _connect_ap(int id);
};

#endif