#include "bullet_spawner.h"

//"overrides"
void BulletSpawner::_notification(int p_what) {
    switch (p_what) {

		case NOTIFICATION_READY: {
			_ready();
		} break;

		case NOTIFICATION_PHYSICS_PROCESS: {
			_physics_process(get_physics_process_delta_time());
		}
        break;

        case NOTIFICATION_TRANSFORM_CHANGED: {
            if (get_global_transform().get_rotation() != _previous_transform.get_rotation()){
                set_global_rotation(0);
                if (inherit_rotation){
                    shots_update_required = true;
                }
            }
            if (get_global_transform().get_scale() != _previous_transform.get_scale()){
                set_global_scale(Vector2(1,1));
                if (inherit_scale){
                    shots_update_required = true;
                }
            }
            _previous_transform = get_global_transform();
        }
        break;

		default:
			break;
	}
}

void BulletSpawner::_ready() {
    set_notify_transform(true);
    if (Engine::get_singleton()->is_editor_hint()){
        set_physics_process(false);
        set_process(true);
        return;
    }
    BulletServerRelay *relay = Object::cast_to<BulletServerRelay>(Engine::get_singleton()->get_singleton_object("BulletServerRelay"));
	connect("bullet_fired", relay, "on_bullet_fired");
	connect("volley_fired", relay, "on_volley_fired");
    set_physics_process(true);
    if (!in_game_preview){
        set_process(false);
    }
}

void BulletSpawner::_process(float delta) {
    update();
}

void BulletSpawner::_physics_process(float delta){
    if (Engine::get_singleton()->is_editor_hint()){
        return;
    }
    if (autofire){
        _autofire_step++;
        if (_autofire_step >= interval_frames){
            fire();
            _autofire_step = 0;
        }
    }
}

//public functions
void BulletSpawner::fire() {
    if (!is_inside_tree() || Engine::get_singleton()->is_editor_hint()){
        return;
    }
    emit_signal("volley_fired", bullet_type->duplicate(), get_global_position(), get_scattered_shots());
}

Array BulletSpawner::get_shots() {
    if (shots_update_required){
        _update_cached_shots();
    }
    return cached_shots;
}

Array BulletSpawner::get_scattered_shots() {
    if (scatter_range <= 0.0){
        return get_shots();
    } 

    Array s_shots = get_shots().duplicate(true);
    float rand_offset;

    switch (scatter_type)
    {
    case VOLLEY:
        rand_offset = Math::randf() * scatter_range - scatter_range / 2;
        for (int i = 0; i < s_shots.size(); i++){
            Dictionary shot_info = s_shots[i];
            Vector2 shot_dir = shot_info["direction"];
            shot_info["direction"] = shot_dir.rotated(rand_offset);
        }
        break;
    
    case BULLET:
        for (int i = 0; i < s_shots.size(); i++){
            rand_offset = Math::randf() * scatter_range - scatter_range / 2;
            Dictionary shot_info = s_shots[i];
            Vector2 shot_dir = shot_info["direction"];
            shot_info["direction"] = shot_dir.rotated(rand_offset);
        }
        break;

    default:
        break;
    }
    return s_shots;
}

//private functions
void BulletSpawner::_update_cached_shots() {
    Array new_shots;

    if (bullet_count == 1 || spread == 0.0){
        Vector2 dir = Vector2(1,0).rotated(spawn_angle + volley_offset * spread / 2);
        Dictionary shot_info;
        shot_info["offset"] = (dir * spawn_radius * get_adjusted_global_scale()).rotated(get_adjusted_global_rotation());
        shot_info["direction"] = dir.rotated(get_adjusted_global_rotation());
        new_shots.push_back(shot_info);
    } else {
        Vector2 arc_start = Vector2(1,0).rotated(-spread / 2);
        Vector2 arc_end = arc_start.rotated(spread);
        float spacing = spread / (bullet_count - 1);
        bool spacing_maxed = false;

        Vector2 volley_start = arc_start;
        if (spacing > 2 * M_PI / bullet_count){
            spacing = 2 * M_PI / bullet_count;
            volley_start = Vector2(-1,0).rotated(spacing / 2);
            spacing_maxed = true;
        }
        
        for (int i = 0; i < bullet_count; i++){
            float shot_angle = spacing * i + volley_offset * spread / 2;
            if (!spacing_maxed){
                shot_angle = Math::wrapf(shot_angle, 0 - spacing / 2, spread + spacing / 2);
            }
            Vector2 shot_direction = volley_start.rotated(shot_angle);
            if (spread > 2 * M_PI || Math::abs(shot_direction.angle()) <= arc_end.angle() + 0.001){
                shot_direction = shot_direction.rotated(spawn_angle);
                Vector2 shot_position = _get_spawn_offset(shot_direction);
                Dictionary shot_info;
                shot_info["offset"] = shot_position;
                shot_info["direction"] = shot_direction.rotated(get_adjusted_global_rotation());
                new_shots.push_back(shot_info);
            }
        }
    }
    cached_shots = new_shots;
    shots_update_required = false;
}

Vector2 BulletSpawner::_get_spawn_offset(const Vector2 &p_shot_dir) {
    return (p_shot_dir * spawn_radius * get_adjusted_global_scale()).rotated(get_adjusted_global_rotation());
}

//setters/getters
void BulletSpawner::set_autofire(bool p_enabled) {
    autofire = p_enabled;
    _autofire_step = 0;
    if (autofire){
        fire();
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

void BulletSpawner::set_bullet_data(const Ref<BulletData> &p_type) {
    bullet_type = p_type;
    shots_update_required = true;
}

Ref<BulletData> BulletSpawner::get_bullet_data() const {
    return bullet_type;
}

void BulletSpawner::set_spawn_radius(float p_radius) {
    spawn_radius = p_radius;
    shots_update_required = true;    
}

float BulletSpawner::get_spawn_radius() const {
    return spawn_radius;
}

void BulletSpawner::set_spawn_angle(float p_radians) {
    spawn_angle = p_radians;
    shots_update_required = true;
}

float BulletSpawner::get_spawn_angle() const {
    return spawn_angle;
}

void BulletSpawner::set_spawn_angle_degrees(float p_degrees) {
    spawn_angle = p_degrees * M_PI/180.0;
    shots_update_required = true;
}

float BulletSpawner::get_spawn_angle_degrees() const {
    return spawn_angle * 180.0/M_PI;
}

void BulletSpawner::set_bullet_count(int p_count) {
    bullet_count = p_count;
    shots_update_required = true;
}

int BulletSpawner::get_bullet_count() const {
    return bullet_count;
}

void BulletSpawner::set_spread(float p_radians){
    spread = p_radians;
    shots_update_required = true;
}

float BulletSpawner::get_spread() const {
    return spread;
}

void BulletSpawner::set_spread_degrees(float p_degrees){
    spread = p_degrees * M_PI/180.0;
    shots_update_required = true;
}

float BulletSpawner::get_spread_degrees() const {
    return spread * 180.0/M_PI;
}

void BulletSpawner::set_volley_offset(float p_offset) {
    volley_offset = p_offset;
    shots_update_required = true;
}

float BulletSpawner::get_volley_offset() const {
    return volley_offset;
}

void BulletSpawner::set_scatter_type(int p_type) {
    scatter_type = p_type;
}

int BulletSpawner::get_scatter_type() const {
    return scatter_type;
}

void BulletSpawner::set_scatter_range(float p_radians) {
    scatter_range = p_radians;
}

float BulletSpawner::get_scatter_range() const {
    return scatter_range;
}

void BulletSpawner::set_scatter_range_degrees(float p_degrees) {
    scatter_range = p_degrees * M_PI/180.0;
}

float BulletSpawner::get_scatter_range_degrees() const {
    return scatter_range * 180.0/M_PI;
}

void BulletSpawner::set_inherit_rotation(bool p_enabled){
    inherit_rotation = p_enabled;
    shots_update_required = true;
}

bool BulletSpawner::get_inherit_rotation() const {
    return inherit_rotation;
}

void BulletSpawner::set_self_rotation(float p_radians) {
    self_rotation = p_radians;
    shots_update_required = true;
}

float BulletSpawner::get_self_rotation() const {
    return self_rotation;
}

void BulletSpawner::set_self_rotation_degrees(float p_degrees) {
    self_rotation = p_degrees * M_PI/180.0;
    shots_update_required = true;
}

float BulletSpawner::get_self_rotation_degrees() const {
    return self_rotation * 180.0/M_PI;
}

void BulletSpawner::set_adjusted_global_rotation(float p_radians) {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_rotation && parent_2D != NULL){
        self_rotation = p_radians - parent_2D->get_global_rotation();
    } else {
        self_rotation = p_radians;
    }
    shots_update_required = true;
}

float BulletSpawner::get_adjusted_global_rotation() const {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_rotation && parent_2D != NULL){
        return self_rotation + parent_2D->get_global_rotation();
    } else {
        return self_rotation;
    }
}

void BulletSpawner::set_inherit_scale(bool p_enabled){
    inherit_scale = p_enabled;
    shots_update_required = true;
}

bool BulletSpawner::get_inherit_scale() const {
    return inherit_scale;
}

void BulletSpawner::set_self_scale(const Vector2 &p_scale) {
    self_scale = p_scale;
    shots_update_required = true;
}

Vector2 BulletSpawner::get_self_scale() const {
    return self_scale;    
}

void BulletSpawner::set_adjusted_global_scale(const Vector2 &p_scale) {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_scale && parent_2D != NULL){
        self_scale = p_scale / parent_2D->get_global_scale();
    } else {
        self_scale = p_scale;
    }
    shots_update_required = true;
}

Vector2 BulletSpawner::get_adjusted_global_scale() const {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_scale && parent_2D != NULL){
        return self_scale * parent_2D->get_global_scale();
    } else {
        return self_scale;
    }
}

//drawing functions
void BulletSpawner::_draw_shot_preview(const Color &p_border_col, const Color &p_shot_col) {
    
}

void BulletSpawner::_draw_adjusted_arc(float p_inner_rad, float p_outer_rad, const Vector2 &p_arc_start, int p_point_count, const Color &p_color) {
    
}

//godot binds
void BulletSpawner::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fire"), &BulletSpawner::fire);
    ClassDB::bind_method(D_METHOD("get_shots"), &BulletSpawner::get_shots);
    ClassDB::bind_method(D_METHOD("get_scattered_shots"), &BulletSpawner::get_scattered_shots);

    ClassDB::bind_method(D_METHOD("set_autofire", "enabled"), &BulletSpawner::set_autofire);
    ClassDB::bind_method(D_METHOD("get_autofire"), &BulletSpawner::get_autofire);

    ClassDB::bind_method(D_METHOD("set_interval_frames", "interval"), &BulletSpawner::set_interval_frames);
    ClassDB::bind_method(D_METHOD("get_interval_frames"), &BulletSpawner::get_interval_frames);

    ClassDB::bind_method(D_METHOD("set_bullet_data", "data"), &BulletSpawner::set_bullet_data);
    ClassDB::bind_method(D_METHOD("get_bullet_data"), &BulletSpawner::get_bullet_data);

    ClassDB::bind_method(D_METHOD("set_spawn_radius", "radius"), &BulletSpawner::set_spawn_radius);
    ClassDB::bind_method(D_METHOD("get_spawn_radius"), &BulletSpawner::get_spawn_radius);

    ClassDB::bind_method(D_METHOD("set_spawn_angle", "radians"), &BulletSpawner::set_spawn_angle);
    ClassDB::bind_method(D_METHOD("get_spawn_angle"), &BulletSpawner::get_spawn_angle);

    ClassDB::bind_method(D_METHOD("set_spawn_angle_degrees", "degrees"), &BulletSpawner::set_spawn_angle_degrees);
    ClassDB::bind_method(D_METHOD("get_spawn_angle_degrees"), &BulletSpawner::get_spawn_angle_degrees);

    ClassDB::bind_method(D_METHOD("set_bullet_count", "count"), &BulletSpawner::set_bullet_count);
    ClassDB::bind_method(D_METHOD("get_bullet_count"), &BulletSpawner::get_bullet_count);

    ClassDB::bind_method(D_METHOD("set_spread", "radians"), &BulletSpawner::set_spread);
    ClassDB::bind_method(D_METHOD("get_spread"), &BulletSpawner::get_spread);

    ClassDB::bind_method(D_METHOD("set_spread_degrees", "degrees"), &BulletSpawner::set_spread_degrees);
    ClassDB::bind_method(D_METHOD("get_spread_degrees"), &BulletSpawner::get_spread_degrees);

    ClassDB::bind_method(D_METHOD("set_volley_offset", "offset"), &BulletSpawner::set_volley_offset);
    ClassDB::bind_method(D_METHOD("get_volley_offset"), &BulletSpawner::get_volley_offset);

    ClassDB::bind_method(D_METHOD("set_scatter_type", "type"), &BulletSpawner::set_scatter_type);
    ClassDB::bind_method(D_METHOD("get_scatter_type"), &BulletSpawner::get_scatter_type);

    ClassDB::bind_method(D_METHOD("set_scatter_range", "radians"), &BulletSpawner::set_scatter_range);
    ClassDB::bind_method(D_METHOD("get_scatter_range"), &BulletSpawner::get_scatter_range);

    ClassDB::bind_method(D_METHOD("set_scatter_range_degrees", "degrees"), &BulletSpawner::set_scatter_range_degrees);
    ClassDB::bind_method(D_METHOD("get_scatter_range_degrees"), &BulletSpawner::get_scatter_range_degrees);

    ClassDB::bind_method(D_METHOD("set_inherit_rotation", "enabled"), &BulletSpawner::set_inherit_rotation);
    ClassDB::bind_method(D_METHOD("get_inherit_rotation"), &BulletSpawner::get_inherit_rotation);

    ClassDB::bind_method(D_METHOD("set_self_rotation", "radians"), &BulletSpawner::set_self_rotation);
    ClassDB::bind_method(D_METHOD("get_self_rotation"), &BulletSpawner::get_self_rotation);

    ClassDB::bind_method(D_METHOD("set_self_rotation_degrees", "degrees"), &BulletSpawner::set_self_rotation_degrees);
    ClassDB::bind_method(D_METHOD("get_self_rotation_degrees"), &BulletSpawner::get_self_rotation_degrees);

    ClassDB::bind_method(D_METHOD("set_adjusted_global_rotation", "radians"), &BulletSpawner::set_adjusted_global_rotation);
    ClassDB::bind_method(D_METHOD("get_adjusted_global_rotation"), &BulletSpawner::get_adjusted_global_rotation);

    ClassDB::bind_method(D_METHOD("set_inherit_scale", "enabled"), &BulletSpawner::set_inherit_scale);
    ClassDB::bind_method(D_METHOD("get_inherit_scale"), &BulletSpawner::get_inherit_scale);

    ClassDB::bind_method(D_METHOD("set_self_scale", "scale"), &BulletSpawner::set_self_scale);
    ClassDB::bind_method(D_METHOD("get_self_scale"), &BulletSpawner::get_self_scale);

    ClassDB::bind_method(D_METHOD("set_adjusted_global_scale", "scale"), &BulletSpawner::set_adjusted_global_scale);
    ClassDB::bind_method(D_METHOD("get_adjusted_global_scale"), &BulletSpawner::get_adjusted_global_scale);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autofire"), "set_autofire", "get_autofire");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "interval_frames", PROPERTY_HINT_RANGE, "1,300,1,or_greater"), "set_interval_frames", "get_interval_frames");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_data", PROPERTY_HINT_RESOURCE_TYPE, "BulletData"), "set_bullet_data", "get_bullet_data");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "spawn_radius", PROPERTY_HINT_RANGE, "0,100,0.01,or_greater"), "set_spawn_radius", "get_spawn_radius");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "spawn_angle_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_spawn_angle_degrees", "get_spawn_angle_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "bullet_count", PROPERTY_HINT_RANGE, "1,100,1,or_greater"), "set_bullet_count", "get_bullet_count");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "spread", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_spread", "get_spread");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "spread_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_spread_degrees", "get_spread_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "volley_offset", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_volley_offset", "get_volley_offset");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scatter_type", PROPERTY_HINT_ENUM, "VOLLEY, BULLET"), "set_scatter_type", "get_scatter_type");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "scatter_range", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_scatter_range", "get_scatter_range");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "scatter_range_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_scatter_range_degrees", "get_scatter_range_degrees");
    ADD_GROUP("Transform Modifiers", "");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "inherit_rotation"), "set_inherit_rotation", "get_inherit_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "self_rotation", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_self_rotation", "get_self_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "self_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_self_rotation_degrees", "get_self_rotation_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "inherit_scale"), "set_inherit_scale", "get_inherit_scale");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "self_scale"), "set_self_scale", "get_self_scale");

    ADD_SIGNAL(MethodInfo("bullet_fired", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::VECTOR2, "direction")));
    ADD_SIGNAL(MethodInfo("volley_fired", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::ARRAY, "shots")));
}

//initialiser/terminator
BulletSpawner::BulletSpawner() {
    autofire = false;
    interval_frames = 10;
    spawn_radius = 0.0;
    spawn_angle = 0.0;
    bullet_count = 1;
    spread = 0.0;
    volley_offset = 0.0;
    scatter_type = 0;
    scatter_range = 0.0;
    inherit_rotation = true;
    self_rotation = 0.0;
    inherit_scale = true;
    self_scale = Vector2(1,1);
    in_game_preview = false;
    preview_color = Color(0.0, 1.0, 0.0, 1.0); //green
}

BulletSpawner::~BulletSpawner() {
    
}