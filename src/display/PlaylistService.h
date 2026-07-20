#pragma once
#include <Arduino.h>
#include <Preferences.h>

class PlaylistService {
 public:
  static constexpr uint8_t SCENE_COUNT = 7;
  void begin();
  bool enabled(uint8_t index) const { return index < SCENE_COUNT && enabled_[index]; }
  uint8_t next(uint8_t current) const;
  String toJson() const;
  bool updateFromJson(const String& json);
 private:
  void save();
  Preferences store_;
  bool enabled_[SCENE_COUNT] = {true,true,true,true,true,true,true};
};
