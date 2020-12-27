#include <vector>
#include <WiFi.h>
#include "Targets.hpp"

std::vector<Target> Targets::_targets;
std::function<void(void)> Targets::_on_init = nullptr;
std::function<void(int)> Targets::_on_hit = nullptr;

Targets::Targets(std::function<void(void)> on_init,
                 std::function<void(int)> on_receive_ir,
                 std::function<void(int)> on_not_receive_ir,
                 std::function<void(int)> on_hit)
    : _on_receive_ir(on_receive_ir)
    , _on_not_receive_ir(on_not_receive_ir)
{
  Targets::_on_init = on_init;
  Targets::_on_hit = on_hit;
}

bool Targets::begin(int unit_id, int targets_num, bool begin_wifi)
{
  for (int i = 0; i < targets_num; i++)
  {
    _targets.push_back(Target(i));
  }
  if (begin_wifi)
  {
    if(_connect_ap(unit_id)){
      Serial.println("connected to wifi");
    }else{
      Serial.println("<ERROR> failed to connect wifi");
    }
  }
  _server.reset(new TargetServer());
  _server->on_shoot(Targets::_handle_shoot);
  _server->on_init(Targets::_handle_init);
  _server->begin();
}

std::vector<int> Targets::get_error_targets(void)
{
  std::vector<int> error_ids;
  for (const auto &target : _targets)
  {
    if (!target.is_connected())
    {
      error_ids.push_back(target.get_id());
    }
  }
  return error_ids;
}

void Targets::update()
{
  _server->handle_client();
  for (const auto &target : _targets)
  {
    if (target.is_recieve_ir())
    {
      _on_receive_ir(target.get_id());
    }
    else
    {
      _on_not_receive_ir(target.get_id());
    }
  }
}

void Targets::_handle_shoot(WebServer *server)
{
  String shoot_gun_num_s = server->arg("gun_num");
  if (shoot_gun_num_s == "")
  {
    _response_to_center(*server, 0);
    return;
  }

  int shoot_gun_num_i = shoot_gun_num_s.toInt();
  for (auto &target : _targets)
  {
    if (!target.is_alive)
    {
      continue;
    }
    if (_is_hit(target, shoot_gun_num_i))
    {
      _response_to_center(*server, shoot_gun_num_i);
      target.is_alive = false;
      Targets::_on_hit(target.get_id());
      return;
    }
  }
  _response_to_center(*server, 0);
  return;
}

void Targets::_handle_init(WebServer *server)
{
  Targets::_on_init();
  for (auto &target : Targets::_targets)
  {
    target.is_alive = true;
  }
  server->send(200, "text/plain", "initialized");
}

void Targets::_response_to_center(WebServer &server, int response_num)
{
  server.send(200, "text/plain", "target=" + String(response_num));
}

bool Targets::_is_hit(const Target &target, int shoot_gun_num_i)
{
  byte gun_num = target.get_gun_num();
  if (gun_num == shoot_gun_num_i)
  {
    return true;
  }
  return false;
}

bool Targets::_connect_ap(int id)
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.config(IPAddress(192, 168, 100, 200 + id),
              IPAddress(192, 168, 100, 1), IPAddress(255, 255, 255, 0));
  const char* ssid = "ssid";
  const char* password = "password";
  WiFi.begin(ssid, password);
  Serial.printf("connecting to wifi ssid = %s, password = %s", ssid, password);
  unsigned int try_connect_count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    try_connect_count++;
    if (try_connect_count > 30) break;
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  return false;
}