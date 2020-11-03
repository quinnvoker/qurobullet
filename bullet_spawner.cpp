#include "bullet_spawner.h"

//"overrides"
void BulletSpawner::_notification(int p_what) {
  switch (p_what) {

  case NOTIFICATION_READY: {
    set_notify_transform(true);
      if (Engine::get_singleton()->is_editor_hint()){
        set_physics_process(false);
        return;
      }
      if (relay_autoconnect) {
        BulletServerRelay *relay = Object::cast_to<BulletServerRelay>(Engine::get_singleton()->get_singleton_object("BulletServerRelay"));
        connect("volley_fired", relay, "spawn_volley");
      }
      set_physics_process(true);
      set_visible(preview_visible_in_game);
  } break;

  case NOTIFICATION_PHYSICS_PROCESS: {
    if (Engine::get_singleton()->is_editor_hint()){
        return;
      }
      if (autofire){
        _autofire_time += get_physics_process_delta_time();
        if (_autofire_time >= interval_frames / ProjectSettings::get_singleton()->get("physics/common/physics_fps").operator float()){
          fire();
          _autofire_time = 0;
        }
      }
  } break;

    case NOTIFICATION_TRANSFORM_CHANGED: {
      if (aim_mode == TARGET_GLOBAL && get_global_transform().get_origin() != _previous_transform.get_origin()) {
        _volley_change_notify();
      } else if (get_global_transform().get_rotation() != _previous_transform.get_rotation()){
        _volley_change_notify();
      } else if (get_global_transform().get_scale() != _previous_transform.get_scale()){
        _volley_change_notify();
      }
      _previous_transform = get_global_transform();
    } break;

    case NOTIFICATION_DRAW: {
      if (is_visible_in_tree()){
        _draw_preview(preview_color, preview_shot_color);
      }
    }

  default:
    break;
  }
}

//public functions
bool BulletSpawner::can_fire() const {
  return !Engine::get_singleton()->is_editor_hint() && is_inside_tree() && bullet_type.is_valid();
}

void BulletSpawner::fire() {
  if (!can_fire()){
    return;
  }
  switch (pattern_mode)
  {
  case ALL: {
    emit_signal("volley_fired", bullet_type->duplicate(), get_global_position(), get_scattered_volley());
  } break;

  case MANUAL: {
    emit_signal("volley_fired", bullet_type->duplicate(), get_global_position(), _get_selected_shots(get_scattered_volley(), active_shot_indices));
  } break;

  default:
    break;
  }
}

void BulletSpawner::fire_shots(const PoolIntArray &p_shot_indices) {
  if (!can_fire()){
    return;
  }
  emit_signal("volley_fired", bullet_type->duplicate(), get_global_position(), _get_selected_shots(get_scattered_volley(), p_shot_indices));
}

Array BulletSpawner::get_volley() {
  if (_volley_changed){
    _cached_volley = _create_volley();
    _volley_changed = false;
  }
  return _cached_volley;
}

Array BulletSpawner::get_scattered_volley() {
  if (scatter_mode == NONE || Math::is_zero_approx(scatter_range)){
    return get_volley();
  }

  Array s_volley = get_volley().duplicate(true);
  float rand_offset;

  switch (scatter_mode)
  {
  case BULLET:
    for (int i = 0; i < s_volley.size(); i++){
      rand_offset = Math::randf() * scatter_range - scatter_range / 2;
      Dictionary shot_info = s_volley[i];
      shot_info["direction"] = Vector2(shot_info["direction"]).rotated(rand_offset);
    }
    break;

  case VOLLEY:
    rand_offset = Math::randf() * scatter_range - scatter_range / 2;
    for (int i = 0; i < s_volley.size(); i++){
      Dictionary shot_info = s_volley[i];
      shot_info["direction"] = Vector2(shot_info["direction"]).rotated(rand_offset);
    }
    break;

  default:
    break;
  }
  return s_volley;
}

//private functions
Array BulletSpawner::_get_selected_shots(const Array &p_volley, const PoolIntArray &p_shot_indices) const {
  Array selected_shots;
  Dictionary used_indices;
  for (int i = 0; i < p_shot_indices.size(); i++){
    int shot_index = p_shot_indices[i];
    if (shot_index > -1 && shot_index < p_volley.size() && !used_indices.has(shot_index)){
      selected_shots.append(p_volley[shot_index]);
      used_indices[shot_index] = true;
    }
  }
  return selected_shots;
}

void BulletSpawner::_volley_change_notify() {
  _volley_changed = true;
#ifdef TOOLS_ENABLED
  update_configuration_warning();
#endif
  if(is_visible_in_tree()){
    update();
  }
}

Array BulletSpawner::_create_volley() const {
  Array volley;
  if (_get_unique_shot_count(true) == 1){
    Vector2 dir = Vector2(1,0).rotated(arc_rotation);
    Dictionary shot;
    shot["normal"] = dir;
    shot["position"] = _get_shot_position(dir);
    shot["direction"] = _get_shot_direction(dir * radius, dir);
    volley.push_back(shot);
    return volley;
  }

  float arc_extent = arc_width / 2;
  float spacing = arc_width / (shot_count - 1);
  bool spacing_maxed = false;

  float volley_start = -arc_extent;
  if (spacing > 2 * Math_PI / shot_count){
    spacing = 2 * Math_PI / shot_count;
    volley_start = -Math_PI + spacing / 2;
    spacing_maxed = true;
  }

  for (int i = 0; i < shot_count; i++){
    float shot_angle = spacing * i + (arc_offset * arc_width / 2);
    if (!spacing_maxed){
      shot_angle = Math::wrapf(shot_angle, 0 - spacing / 2, arc_width + spacing / 2);
    }
    shot_angle += volley_start;
    Vector2 shot_normal = Vector2(1,0).rotated(shot_angle);
    if (arc_width >= 2 * Math_PI || Math::abs(shot_normal.angle()) <= arc_extent + 0.001){
      Dictionary shot;
      shot_normal = shot_normal.rotated(arc_rotation);
      shot["normal"] = shot_normal;
      shot["position"] = _get_shot_position(shot_normal);
      shot["direction"] = _get_shot_direction(shot["position"], shot_normal);
      volley.push_back(shot);
    }
  }
  return volley;
}

Vector2 BulletSpawner::_get_shot_position(const Vector2 &p_normal) const{
  return (p_normal * radius * get_global_scale()).rotated(get_global_rotation());
}

Vector2 BulletSpawner::_get_shot_direction(const Vector2 &p_position, const Vector2 &p_normal) const{
  Vector2 direction;
  switch (aim_mode){
    case RADIAL: {
      direction = p_normal.rotated(aim_angle + get_global_rotation());
    } break;
    case UNIFORM: {
      direction = Vector2(1,0).rotated(aim_angle + get_global_rotation());
    } break;
    case TARGET_RELATIVE: {
      direction = (aim_target_position - p_position).normalized();
    } break;
    case TARGET_GLOBAL: {
      direction = (aim_target_position - (get_global_position() + p_position)).normalized();
    } break;
    default:{
    } break;
  }
  //if calculated direction is not a normal, such as when targeted aim modes aim at their own current position, use the initial shot normal instead
  if (!direction.is_normalized()){
    direction = p_normal;
  }
  return direction;
}

int BulletSpawner::_get_unique_shot_count(bool p_include_scatter) const {
  //returns number of shots that will follow a unique path (or have potential to, with scatter) after firing
  //used in checks to prevent an entire volley from "stacking" by being spawned in the same position with the same direction
  if (shot_count <= 1){
    return 1;
  }
  if (Math::is_zero_approx(arc_width) || (aim_mode != RADIAL && Math::is_zero_approx(radius))) {
    if (p_include_scatter && scatter_mode == BULLET && !Math::is_zero_approx(scatter_range)){
      return shot_count;
    }
    return 1;
  }
  return shot_count;
}

//setters/getters
void BulletSpawner::set_autofire(bool p_enabled) {
  autofire = p_enabled;
  if (autofire){
    _autofire_time = INFINITY;
  } else {
    _autofire_time = 0;
  }
}

bool BulletSpawner::get_autofire() const {
  return autofire;
}

void BulletSpawner::set_interval_frames(int p_interval) {
  interval_frames = p_interval;
}

int BulletSpawner::get_interval_frames() const {
  return interval_frames;
}

void BulletSpawner::set_bullet_type(const Ref<BulletType> &p_type) {
  bullet_type = p_type;
  _volley_change_notify();
}

Ref<BulletType> BulletSpawner::get_bullet_type() const {
  return bullet_type;
}

void BulletSpawner::set_shot_count(int p_count) {
  shot_count = p_count;
  _volley_change_notify();
}

int BulletSpawner::get_shot_count() const {
  return shot_count;
}

void BulletSpawner::set_radius(float p_radius) {
  radius = p_radius;
  _volley_change_notify();
}

float BulletSpawner::get_radius() const {
  return radius;
}

void BulletSpawner::set_arc_width(float p_radians){
  arc_width = p_radians;
  _volley_change_notify();
}

float BulletSpawner::get_arc_width() const {
  return arc_width;
}

void BulletSpawner::set_arc_width_degrees(float p_degrees){
  arc_width = Math::deg2rad(p_degrees);
  _volley_change_notify();
}

float BulletSpawner::get_arc_width_degrees() const {
  return Math::rad2deg(arc_width);
}

void BulletSpawner::set_arc_rotation(float p_radians) {
  arc_rotation = p_radians;
  _volley_change_notify();
}

float BulletSpawner::get_arc_rotation() const {
  return arc_rotation;
}

void BulletSpawner::set_arc_rotation_degrees(float p_degrees) {
  arc_rotation = Math::deg2rad(p_degrees);
  _volley_change_notify();
}

float BulletSpawner::get_arc_rotation_degrees() const {
  return Math::rad2deg(arc_rotation);
}

void BulletSpawner::set_arc_offset(float p_offset) {
  arc_offset = p_offset;
  _volley_change_notify();
}

float BulletSpawner::get_arc_offset() const {
  return arc_offset;
}

void BulletSpawner::set_aim_mode(AimMode p_mode){
  aim_mode = p_mode;
  _volley_change_notify();
  _change_notify();
}

BulletSpawner::AimMode BulletSpawner::get_aim_mode() const {
  return aim_mode;
}


void BulletSpawner::set_aim_angle(float p_radians){
  aim_angle = p_radians;
  if (aim_mode == RADIAL || aim_mode == UNIFORM){
    _volley_change_notify();
  }
}

float BulletSpawner::get_aim_angle() const {
  return aim_angle;
}

void BulletSpawner::set_aim_angle_degrees(float p_degrees) {
  set_aim_angle(Math::deg2rad(p_degrees));
}

float BulletSpawner::get_aim_angle_degrees() const {
  return Math::rad2deg(aim_angle);
}

void BulletSpawner::set_aim_target_position(const Vector2 &p_position) {
  aim_target_position = p_position;
  if (aim_mode == TARGET_RELATIVE || aim_mode == TARGET_GLOBAL) {
    _volley_change_notify();
  }
}

Vector2 BulletSpawner::get_aim_target_position() const {
  return aim_target_position;
}

void BulletSpawner::set_scatter_mode(ScatterMode p_mode) {
  scatter_mode = p_mode;
  _change_notify();
}

BulletSpawner::ScatterMode BulletSpawner::get_scatter_mode() const {
  return scatter_mode;
}

void BulletSpawner::set_scatter_range(float p_radians) {
  scatter_range = p_radians;
}

float BulletSpawner::get_scatter_range() const {
  return scatter_range;
}

void BulletSpawner::set_scatter_range_degrees(float p_degrees) {
  scatter_range = Math::deg2rad(p_degrees);
}

float BulletSpawner::get_scatter_range_degrees() const {
  return Math::rad2deg(scatter_range);
}

void BulletSpawner::set_pattern_mode(PatternMode p_mode) {
  pattern_mode = p_mode;
  _volley_change_notify();
  _change_notify();
}

BulletSpawner::PatternMode BulletSpawner::get_pattern_mode() const {
  return pattern_mode;
}

void BulletSpawner::set_active_shot_indices(const PoolIntArray &p_points) {
  active_shot_indices = p_points;
  _volley_change_notify();
}

PoolIntArray BulletSpawner::get_active_shot_indices() const{
  return active_shot_indices;
}

void BulletSpawner::set_preview_visible_in_game(bool p_enabled){
  preview_visible_in_game = p_enabled;
  if (is_visible_in_tree()){
    update();
  }
}

bool BulletSpawner::get_preview_visible_in_game() const {
  return preview_visible_in_game;
}

void BulletSpawner::set_preview_color(const Color &p_color){
  preview_color = p_color;
  if (is_visible_in_tree()) {
    update();
  }
}

Color BulletSpawner::get_preview_color() const {
  return preview_color;
}

void BulletSpawner::set_preview_shot_color(const Color &p_color){
  preview_shot_color = p_color;
  if (is_visible_in_tree()) {
    update();
  }
}

Color BulletSpawner::get_preview_shot_color() const {
  return preview_shot_color;
}

void BulletSpawner::set_preview_extent(float p_length){
  preview_extent = p_length;
  if (is_visible_in_tree()) {
    update();
  }
}

float BulletSpawner::get_preview_extent() const {
  return preview_extent;
}

void BulletSpawner::set_preview_arc_points(int p_count){
  preview_arc_points = p_count;
  if (is_visible_in_tree()) {
    update();
  }
}

int BulletSpawner::get_preview_arc_points() const {
  return preview_arc_points;
}

void BulletSpawner::set_relay_autoconnect(bool p_enabled) {
  relay_autoconnect = p_enabled;
}

bool BulletSpawner::get_relay_autoconnect() const {
  return relay_autoconnect;
}

//drawing functions
void BulletSpawner::_draw_preview(const Color &p_border_col, const Color &p_shot_col) {
  Color dim_border_col = Color(p_border_col.r, p_border_col.g, p_border_col.b, 0.25);
  Color dim_shot_col = Color(p_shot_col.r, p_shot_col.g, p_shot_col.b, 0.25);

  if (_get_unique_shot_count() > 1) {
    Vector<Vector2> inner_points;
    Vector<Vector2> outer_points;
    inner_points.resize(preview_arc_points);
    outer_points.resize(preview_arc_points);
    float arc_extent = arc_width / 2;
    for (int i = 0; i < inner_points.size(); i++) {
      Vector2 normal = Vector2(cos(-arc_extent), sin(-arc_extent)).rotated(arc_width / (inner_points.size() - 1) * i + arc_rotation);
      Vector2 inner_point = normal * radius;
      Vector2 outer_point = _get_outer_preview_point(inner_point, normal, preview_extent);
      inner_points.set(i, inner_point);
      outer_points.set(i, outer_point);
    }
    if (arc_width < 2* Math_PI){
      outer_points.insert(0, inner_points[0]);
      outer_points.push_back(inner_points[inner_points.size() - 1]);
      draw_polyline(inner_points, p_border_col);
      draw_polyline(outer_points, dim_border_col);
    } else {
      draw_polyline(inner_points, p_border_col);
      draw_polyline(outer_points, dim_border_col);
    }
  }


  Vector2 crosshair_normal = Vector2(1,0).rotated(arc_rotation);
  Vector2 crosshair_inner_point = crosshair_normal * radius;
  Vector2 crosshair_outer_point = _get_outer_preview_point(crosshair_inner_point, crosshair_normal, preview_extent);
  draw_line(Vector2(), crosshair_inner_point, p_border_col);
  draw_line(crosshair_outer_point, _get_outer_preview_point(crosshair_inner_point, crosshair_normal, preview_extent + preview_extent / 5), p_border_col);
  draw_line(crosshair_inner_point, crosshair_outer_point, dim_border_col);

  if (_get_unique_shot_count() > 1){
    _draw_shot_lines(get_volley(), preview_extent, dim_shot_col);
    if (pattern_mode == MANUAL) {
      _draw_shot_lines(_get_selected_shots(get_volley(), active_shot_indices), preview_extent / 5, p_shot_col);
    } else {
      _draw_shot_lines(get_volley(), preview_extent / 5, p_shot_col);
    }
  } else {
    draw_line(crosshair_inner_point, crosshair_outer_point, dim_shot_col);
    draw_line(crosshair_inner_point, crosshair_inner_point + (crosshair_outer_point - crosshair_inner_point) / 5, p_shot_col);
  }

  if (aim_mode == TARGET_RELATIVE) {
    draw_circle(aim_target_position.rotated(-get_global_rotation()) / get_global_scale(), 1, p_border_col);
  } else if (aim_mode == TARGET_GLOBAL) {
    draw_circle((aim_target_position - get_global_position()).rotated(-get_global_rotation()) / get_global_scale(), 1, p_border_col);
  }
}

Vector2 BulletSpawner::_get_outer_preview_point(const Vector2 &p_inner_point, const Vector2 &p_inner_normal, float p_extent) const {
  Vector2 outer_point;
  switch (aim_mode) {
    case RADIAL: {
      outer_point = p_inner_point + p_inner_normal.rotated(aim_angle) * p_extent / get_global_scale();
    } break;

    case UNIFORM: {
      outer_point = p_inner_point + Vector2(1,0).rotated(aim_angle) * p_extent / get_global_scale();
    } break;

    case TARGET_RELATIVE: {
      Vector2 transformed_inner = p_inner_point * get_global_scale();
      Vector2 local_target = aim_target_position.rotated(-get_global_rotation());
      if (transformed_inner.distance_to(local_target) < p_extent){
        outer_point = local_target / get_global_scale();
      } else {
        outer_point = p_inner_point + (local_target - transformed_inner).normalized() * p_extent / get_global_scale();
      }
    } break;

    case TARGET_GLOBAL: {
      Vector2 global_inner_point = get_global_position() + (p_inner_point * get_global_scale()).rotated(get_global_rotation());
      Vector2 local_target = (aim_target_position - get_global_position()).rotated(-get_global_rotation()) / get_global_scale();
      if (global_inner_point.distance_to(aim_target_position) < p_extent){
        outer_point = local_target;
      } else {
        outer_point = p_inner_point + (aim_target_position - global_inner_point).rotated(-get_global_rotation()).normalized() * p_extent / get_global_scale();
      }
    } break;

    default:
      break;
  }
  return outer_point;
}

void BulletSpawner::_draw_shot_lines(const Array &p_volley, float p_length, const Color &p_color) {
  for (int i = 0; i < p_volley.size(); i++){
    Dictionary shot = p_volley[i];
    Vector2 normal = shot["normal"];
    Vector2 position = shot["position"];
    Vector2 local_position = position.rotated(-get_global_rotation()) / get_global_scale();
    draw_line(local_position, _get_outer_preview_point(local_position, normal, p_length), p_color);
  }
}

void BulletSpawner::_validate_property(PropertyInfo &property) const{
  if (property.name == "scatter_range_degrees" && scatter_mode == NONE){
    property.usage = 0;
  }

  if (property.name == "aim_target_position" && !(aim_mode == TARGET_RELATIVE || aim_mode == TARGET_GLOBAL)){
    property.usage = PROPERTY_USAGE_NOEDITOR;
  }

  if (property.name == "aim_angle_degrees" && !(aim_mode == RADIAL || aim_mode == UNIFORM)){
    property.usage = PROPERTY_USAGE_NOEDITOR;
  }

  if (property.name == "active_shot_indices" && pattern_mode != MANUAL){
    property.usage = PROPERTY_USAGE_NOEDITOR;
  }
}

String BulletSpawner::get_configuration_warning() const {
  String warning;
  if (bullet_type.is_null()){
    warning += TTR("This BulletSpawner has no BulletType configured, and will not be able to fire bullets.\nConsider defining one in the BulletSpawner's properties.");
  }

  if (pattern_mode == MANUAL && !Math::is_zero_approx(arc_offset)){
    if (warning != String()){
      warning += "\n\n";
    }
    warning += TTR("This BulletSpawner has a non-zero arc_offset while in MANUAL pattern mode. Shot indices may shift unexpectedly.\nConsider setting the pattern mode to ALL or the arc_offset to zero, depending on your needs.");
  }

  if (aim_mode != RADIAL && Math::is_zero_approx(radius)){
    if (warning != String()){
      warning += "\n\n";
    }
    warning += TTR("This BulletSpawner has a radius of zero, and aim_mode is not RADIAL.\nWith this configuration, only one bullet will be fired, regardless of bullet count and arc width, because all bullets would have identical flight paths.");
  }

  return warning;
}

//godot binds
void BulletSpawner::_bind_methods() {
  ClassDB::bind_method(D_METHOD("can_fire"), &BulletSpawner::can_fire);
  ClassDB::bind_method(D_METHOD("fire"), &BulletSpawner::fire);
  ClassDB::bind_method(D_METHOD("fire_shots", "shot_indices"), &BulletSpawner::fire_shots);

  ClassDB::bind_method(D_METHOD("get_volley"), &BulletSpawner::get_volley);
  ClassDB::bind_method(D_METHOD("get_scattered_volley"), &BulletSpawner::get_scattered_volley);

  ClassDB::bind_method(D_METHOD("set_autofire", "enabled"), &BulletSpawner::set_autofire);
  ClassDB::bind_method(D_METHOD("get_autofire"), &BulletSpawner::get_autofire);

  ClassDB::bind_method(D_METHOD("set_interval_frames", "interval"), &BulletSpawner::set_interval_frames);
  ClassDB::bind_method(D_METHOD("get_interval_frames"), &BulletSpawner::get_interval_frames);

  ClassDB::bind_method(D_METHOD("set_shot_count", "count"), &BulletSpawner::set_shot_count);
  ClassDB::bind_method(D_METHOD("get_shot_count"), &BulletSpawner::get_shot_count);

  ClassDB::bind_method(D_METHOD("set_radius", "radius"), &BulletSpawner::set_radius);
  ClassDB::bind_method(D_METHOD("get_radius"), &BulletSpawner::get_radius);

  ClassDB::bind_method(D_METHOD("set_arc_width", "radians"), &BulletSpawner::set_arc_width);
  ClassDB::bind_method(D_METHOD("get_arc_width"), &BulletSpawner::get_arc_width);

  ClassDB::bind_method(D_METHOD("set_arc_width_degrees", "degrees"), &BulletSpawner::set_arc_width_degrees);
  ClassDB::bind_method(D_METHOD("get_arc_width_degrees"), &BulletSpawner::get_arc_width_degrees);

  ClassDB::bind_method(D_METHOD("set_arc_rotation", "radians"), &BulletSpawner::set_arc_rotation);
  ClassDB::bind_method(D_METHOD("get_arc_rotation"), &BulletSpawner::get_arc_rotation);

  ClassDB::bind_method(D_METHOD("set_arc_rotation_degrees", "degrees"), &BulletSpawner::set_arc_rotation_degrees);
  ClassDB::bind_method(D_METHOD("get_arc_rotation_degrees"), &BulletSpawner::get_arc_rotation_degrees);

  ClassDB::bind_method(D_METHOD("set_arc_offset", "offset"), &BulletSpawner::set_arc_offset);
  ClassDB::bind_method(D_METHOD("get_arc_offset"), &BulletSpawner::get_arc_offset);

  ClassDB::bind_method(D_METHOD("set_bullet_type", "type"), &BulletSpawner::set_bullet_type);
  ClassDB::bind_method(D_METHOD("get_bullet_type"), &BulletSpawner::get_bullet_type);

  ClassDB::bind_method(D_METHOD("set_aim_mode", "mode"), &BulletSpawner::set_aim_mode);
  ClassDB::bind_method(D_METHOD("get_aim_mode"), &BulletSpawner::get_aim_mode);

  ClassDB::bind_method(D_METHOD("set_aim_angle", "radians"), &BulletSpawner::set_aim_angle);
  ClassDB::bind_method(D_METHOD("get_aim_angle"), &BulletSpawner::get_aim_angle);

  ClassDB::bind_method(D_METHOD("set_aim_angle_degrees", "degrees"), &BulletSpawner::set_aim_angle_degrees);
  ClassDB::bind_method(D_METHOD("get_aim_angle_degrees"), &BulletSpawner::get_aim_angle_degrees);

  ClassDB::bind_method(D_METHOD("set_aim_target_position", "mode"), &BulletSpawner::set_aim_target_position);
  ClassDB::bind_method(D_METHOD("get_aim_target_position"), &BulletSpawner::get_aim_target_position);

  ClassDB::bind_method(D_METHOD("set_scatter_mode", "type"), &BulletSpawner::set_scatter_mode);
  ClassDB::bind_method(D_METHOD("get_scatter_mode"), &BulletSpawner::get_scatter_mode);

  ClassDB::bind_method(D_METHOD("set_scatter_range", "radians"), &BulletSpawner::set_scatter_range);
  ClassDB::bind_method(D_METHOD("get_scatter_range"), &BulletSpawner::get_scatter_range);

  ClassDB::bind_method(D_METHOD("set_scatter_range_degrees", "degrees"), &BulletSpawner::set_scatter_range_degrees);
  ClassDB::bind_method(D_METHOD("get_scatter_range_degrees"), &BulletSpawner::get_scatter_range_degrees);

  ClassDB::bind_method(D_METHOD("set_pattern_mode", "mode"), &BulletSpawner::set_pattern_mode);
  ClassDB::bind_method(D_METHOD("get_pattern_mode"), &BulletSpawner::get_pattern_mode);

  ClassDB::bind_method(D_METHOD("set_active_shot_indices", "mode"), &BulletSpawner::set_active_shot_indices);
  ClassDB::bind_method(D_METHOD("get_active_shot_indices"), &BulletSpawner::get_active_shot_indices);

  ClassDB::bind_method(D_METHOD("set_preview_visible_in_game", "mode"), &BulletSpawner::set_preview_visible_in_game);
  ClassDB::bind_method(D_METHOD("get_preview_visible_in_game"), &BulletSpawner::get_preview_visible_in_game);

  ClassDB::bind_method(D_METHOD("set_preview_color", "mode"), &BulletSpawner::set_preview_color);
  ClassDB::bind_method(D_METHOD("get_preview_color"), &BulletSpawner::get_preview_color);

  ClassDB::bind_method(D_METHOD("set_preview_shot_color", "mode"), &BulletSpawner::set_preview_shot_color);
  ClassDB::bind_method(D_METHOD("get_preview_shot_color"), &BulletSpawner::get_preview_shot_color);

  ClassDB::bind_method(D_METHOD("set_preview_extent", "mode"), &BulletSpawner::set_preview_extent);
  ClassDB::bind_method(D_METHOD("get_preview_extent"), &BulletSpawner::get_preview_extent);

  ClassDB::bind_method(D_METHOD("set_preview_arc_points", "mode"), &BulletSpawner::set_preview_arc_points);
  ClassDB::bind_method(D_METHOD("get_preview_arc_points"), &BulletSpawner::get_preview_arc_points);

  ClassDB::bind_method(D_METHOD("set_relay_autoconnect", "allow_incoming"), &BulletSpawner::set_relay_autoconnect);
  ClassDB::bind_method(D_METHOD("get_relay_autoconnect"), &BulletSpawner::get_relay_autoconnect);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autofire"), "set_autofire", "get_autofire");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "interval_frames", PROPERTY_HINT_RANGE, "1,300,or_greater"), "set_interval_frames", "get_interval_frames");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "shot_count", PROPERTY_HINT_RANGE, "1,100,or_greater"), "set_shot_count", "get_shot_count");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius", PROPERTY_HINT_RANGE, "0,500,0.01,or_greater"), "set_radius", "get_radius");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_width", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_arc_width", "get_arc_width");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_width_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_arc_width_degrees", "get_arc_width_degrees");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_rotation", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_arc_rotation", "get_arc_rotation");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_arc_rotation_degrees", "get_arc_rotation_degrees");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_offset", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_arc_offset", "get_arc_offset");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), "set_bullet_type", "get_bullet_type");
  ADD_GROUP("Aim", "aim_");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_mode", PROPERTY_HINT_ENUM, "Radial,Uniform,Relative Target,Global Target"), "set_aim_mode", "get_aim_mode");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "aim_angle", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_aim_angle", "get_aim_angle");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "aim_angle_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_aim_angle_degrees", "get_aim_angle_degrees");
  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "aim_target_position"), "set_aim_target_position", "get_aim_target_position");
  ADD_GROUP("Scatter", "scatter_");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "scatter_mode", PROPERTY_HINT_ENUM, "None,Bullet,Volley"), "set_scatter_mode", "get_scatter_mode");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "scatter_range", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_scatter_range", "get_scatter_range");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "scatter_range_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_scatter_range_degrees", "get_scatter_range_degrees");
  ADD_GROUP("Pattern", "");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "pattern_mode", PROPERTY_HINT_ENUM, "All,Manual"), "set_pattern_mode", "get_pattern_mode");
  ADD_PROPERTY(PropertyInfo(Variant::POOL_INT_ARRAY, "active_shot_indices"), "set_active_shot_indices", "get_active_shot_indices");
  ADD_GROUP("Preview", "preview_");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preview_visible_in_game"), "set_preview_visible_in_game", "get_preview_visible_in_game");
  ADD_PROPERTY(PropertyInfo(Variant::COLOR, "preview_color"), "set_preview_color", "get_preview_color");
  ADD_PROPERTY(PropertyInfo(Variant::COLOR, "preview_shot_color"), "set_preview_shot_color", "get_preview_shot_color");
  ADD_PROPERTY(PropertyInfo(Variant::REAL, "preview_extent", PROPERTY_HINT_RANGE, "0,500,0.1,or_greater"), "set_preview_extent", "get_preview_extent");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "preview_arc_points", PROPERTY_HINT_RANGE, "2,128,or_greater"), "set_preview_arc_points", "get_preview_arc_points");
  ADD_GROUP("Relay", "relay_");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "relay_autoconnect"), "set_relay_autoconnect", "get_relay_autoconnect");

  ADD_SIGNAL(MethodInfo("volley_fired", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "origin"), PropertyInfo(Variant::ARRAY, "volley")));

  BIND_ENUM_CONSTANT(ALL);
  BIND_ENUM_CONSTANT(MANUAL);

  BIND_ENUM_CONSTANT(RADIAL);
  BIND_ENUM_CONSTANT(UNIFORM);
  BIND_ENUM_CONSTANT(TARGET_RELATIVE);
  BIND_ENUM_CONSTANT(TARGET_GLOBAL);

  BIND_ENUM_CONSTANT(NONE);
  BIND_ENUM_CONSTANT(BULLET);
  BIND_ENUM_CONSTANT(VOLLEY);
}

//initialiser/terminator
BulletSpawner::BulletSpawner() {
  autofire = false;
  interval_frames = 10;
  shot_count = 1;
  radius = 10.0;
  arc_width = 0.0;
  arc_rotation = 0.0;
  arc_offset = 0.0;
  aim_mode = RADIAL;
  aim_angle = 0.0;
  scatter_mode = NONE;
  scatter_range = 0.0;
  pattern_mode = ALL;
  preview_visible_in_game = false;
  preview_color = Color(0.0, 1.0, 0.0, 1.0); //green
  preview_shot_color = Color(1.0, 1.0, 1.0, 1.0);
  preview_arc_points = 32;
  preview_extent = 50;
  relay_autoconnect = true;
}

BulletSpawner::~BulletSpawner() {

}
