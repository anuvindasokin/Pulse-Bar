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
  bool use24Hour() const { return use24Hour_; }
  uint8_t durationSeconds() const { return durationSeconds_; }
  const String& transition() const { return transition_; }
 private:
  void save();
  Preferences store_;
  bool enabled_[SCENE_COUNT] = {true,true,true,true,true,true,true};
  bool use24Hour_ = true;
  uint8_t durationSeconds_ = 8;
  String transition_ = "instant";
};
