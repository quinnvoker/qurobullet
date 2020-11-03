#ifndef BULLETSPAWNER_H
#define BULLETSPAWNER_H

#include "core/engine.h"
#include "scene/2d/node_2d.h"
#include "bullet_type.h"
#include "bullet_server_relay.h"

class BulletSpawner : public Node2D{
  GDCLASS(BulletSpawner, Node2D);

public:
  enum PatternMode {
    ALL,
    MANUAL,
  };

  enum AimMode {
    RADIAL,
    UNIFORM,
    TARGET_RELATIVE,
    TARGET_GLOBAL,
  };

  enum ScatterMode {
    NONE,
    BULLET,
    VOLLEY,
  };

private:
  bool autofire;
  float _autofire_time;
  int interval_frames;

  int shot_count;

  float radius;
  float arc_width;
  float arc_rotation;
  float arc_offset;

  Ref<BulletType> bullet_type;

  AimMode aim_mode;
  float aim_angle;
  Vector2 aim_target_position;

  ScatterMode scatter_mode;
  float scatter_range;

  PatternMode pattern_mode;
  PoolIntArray active_shot_indices;

  bool preview_visible_in_game;
  Color preview_color;
  Color preview_shot_color;
  float preview_extent;
  int preview_arc_points;

  bool relay_autoconnect;

  Array _cached_volley;
  bool _volley_changed;

  Transform2D _previous_transform;

  Array _get_selected_shots(const Array &p_volley, const PoolIntArray &p_shot_indices) const;

  void _volley_change_notify();

  Array _create_volley() const;
  Vector2 _get_shot_position(const Vector2 &p_normal) const;
  Vector2 _get_shot_direction(const Vector2 &p_position, const Vector2 &p_normal) const;

  int _get_unique_shot_count(bool p_include_scatter = false) const;

  void _draw_preview(const Color &p_border_col, const Color &p_shot_col);
  Vector2 _get_outer_preview_point(const Vector2 &p_inner_point, const Vector2 &p_inner_normal, float p_extent) const;
  void _draw_shot_lines(const Array &p_volley, float p_length, const Color &p_color);

protected:
  static void _bind_methods();
  void _notification(int p_what);
  void _validate_property(PropertyInfo &property) const;

public:
  bool can_fire() const;

  void fire();
  void fire_shots(const PoolIntArray &p_shot_indices);

  Array get_volley();
  Array get_scattered_volley();

  void set_autofire(bool p_enabled);
  bool get_autofire() const;

  void set_interval_frames(int p_interval);
  int get_interval_frames() const;

  void set_shot_count(int p_count);
  int get_shot_count() const;

  void set_radius(float p_radius);
  float get_radius() const;

  void set_arc_width(float p_radians);
  float get_arc_width() const;

  void set_arc_width_degrees(float p_degrees);
  float get_arc_width_degrees() const;

  void set_arc_rotation(float p_radians);
  float get_arc_rotation() const;

  void set_arc_rotation_degrees(float p_degrees);
  float get_arc_rotation_degrees() const;

  void set_arc_offset(float p_offset);
  float get_arc_offset() const;

  void set_bullet_type(const Ref<BulletType> &p_type);
  Ref<BulletType> get_bullet_type() const;

  void set_aim_mode(AimMode p_mode);
  AimMode get_aim_mode() const;

  void set_aim_angle(float p_radians);
  float get_aim_angle() const;

  void set_aim_angle_degrees(float p_degrees);
  float get_aim_angle_degrees() const;

  void set_aim_target_position(const Vector2 &p_position);
  Vector2 get_aim_target_position() const;

  void set_scatter_mode(ScatterMode p_mode);
  ScatterMode get_scatter_mode() const;

  void set_scatter_range(float p_radians);
  float get_scatter_range() const;

  void set_scatter_range_degrees(float p_degrees);
  float get_scatter_range_degrees() const;

  void set_pattern_mode(PatternMode p_mode);
  PatternMode get_pattern_mode() const;

  void set_active_shot_indices(const PoolIntArray &p_points);
  PoolIntArray get_active_shot_indices() const;

  void set_preview_visible_in_game(bool p_enabled);
  bool get_preview_visible_in_game() const;

  void set_preview_color(const Color &p_color);
  Color get_preview_color() const;

  void set_preview_shot_color(const Color &p_color);
  Color get_preview_shot_color() const;

  void set_preview_extent(float p_length);
  float get_preview_extent() const;

  void set_preview_arc_points(int p_count);
  int get_preview_arc_points() const;

  void set_relay_autoconnect(bool p_enabled);
	bool get_relay_autoconnect() const;

  String get_configuration_warning() const;

  BulletSpawner();
  ~BulletSpawner();
};

VARIANT_ENUM_CAST(BulletSpawner::PatternMode)
VARIANT_ENUM_CAST(BulletSpawner::AimMode)
VARIANT_ENUM_CAST(BulletSpawner::ScatterMode)

#endif
