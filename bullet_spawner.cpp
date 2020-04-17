#include "bullet_spawner.h"

//"overrides"
void BulletSpawner::_notification(int p_what) {
    switch (p_what) {

		case NOTIFICATION_READY: {
			_ready();
		} break;

        case NOTIFICATION_INTERNAL_PROCESS: {
            _process_internal(get_process_delta_time());
        }

		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			_physics_process_internal(get_physics_process_delta_time());
		}
        break;

        case NOTIFICATION_TRANSFORM_CHANGED: {
            if (aim_mode == TARGET_GLOBAL && get_global_transform().get_origin() != _previous_transform.get_origin()) {
                _cache_update_required = true;
            }
            if (get_global_transform().get_rotation() != _previous_transform.get_rotation()){
                set_global_rotation(0);
                if (inherit_rotation){
                    _cache_update_required = true;
                }
            }
            if (get_global_transform().get_scale() != _previous_transform.get_scale()){
                set_global_scale(Vector2(1,1));
                if (inherit_scale){
                    _cache_update_required = true;
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
        set_physics_process_internal(false);
        set_process_internal(true);
        return;
    }
    BulletServerRelay *relay = Object::cast_to<BulletServerRelay>(Engine::get_singleton()->get_singleton_object("BulletServerRelay"));
	connect("bullet_fired", relay, "on_bullet_fired");
	connect("volley_fired", relay, "on_volley_fired");
    set_physics_process_internal(true);
    if (!in_game_preview){
        set_process_internal(false);
    }
}

void BulletSpawner::_process_internal(float delta) {
    update();
}

void BulletSpawner::_physics_process_internal(float delta){
    if (Engine::get_singleton()->is_editor_hint()){
        return;
    }
    if (autofire){
        _autofire_time += delta;
        if (_autofire_time >= interval_frames / ProjectSettings::get_singleton()->get("physics/common/physics_fps").operator float()){
            fire();
            _autofire_time = 0;
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
    if (_cache_update_required){
        _update_cached_shots();
    }
    return _cached_shots;
}

Array BulletSpawner::get_scattered_shots() {
    if (scatter_type == NONE || scatter_range <= 0.0){
        return get_shots();
    } 

    Array s_shots = get_shots().duplicate(true);
    float rand_offset;

    switch (scatter_type)
    {
    case BULLET:
        for (int i = 0; i < s_shots.size(); i++){
            rand_offset = Math::randf() * scatter_range - scatter_range / 2;
            Dictionary shot_info = s_shots[i];
            Vector2 shot_dir = shot_info["direction"];
            shot_info["direction"] = shot_dir.rotated(rand_offset);
        }
        break;
    
    case VOLLEY:
        rand_offset = Math::randf() * scatter_range - scatter_range / 2;
        for (int i = 0; i < s_shots.size(); i++){
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
    Array new_volley = _create_volley();
    _cached_shots = new_volley;
    _cache_update_required = false;
}

Array BulletSpawner::_create_volley() const {
    Array volley;
    if (bullet_count == 1 || (spread == 0.0 && !(scatter_type == BULLET && radius > 0))){
        Vector2 dir = Vector2(1,0).rotated(arc_rotation + get_adjusted_global_rotation());
        Dictionary shot;
        shot["offset"] = dir * radius;
        shot["direction"] = _get_shot_direction(dir * radius, dir);
        volley.push_back(shot);
        return volley;
    }

    float arc_extent = spread / 2;
    float spacing = spread / (bullet_count - 1);
    bool spacing_maxed = false;

    float volley_start = -arc_extent;
    if (spacing > 2 * M_PI / bullet_count){
        spacing = 2 * M_PI / bullet_count;
        volley_start = -M_PI + spacing / 2;
        spacing_maxed = true;
    }

    for (int i = 0; i < bullet_count; i++){
        float shot_angle = spacing * i + (arc_offset * spread / 2);
        if (!spacing_maxed){
            shot_angle = Math::wrapf(shot_angle, 0 - spacing / 2, spread + spacing / 2);
        }
        shot_angle += volley_start;
        if (spread >= 2 * M_PI || Math::abs(Math::fmod(shot_angle, float(M_PI))) <= arc_extent + 0.001){
            Dictionary shot;
            Vector2 shot_normal = Vector2(1,0).rotated(shot_angle + arc_rotation);
            shot["offset"] = (shot_normal * radius * get_adjusted_global_scale()).rotated(get_adjusted_global_rotation());
            shot["direction"] = _get_shot_direction(shot["offset"], shot_normal);
            volley.push_back(shot);
        }
        else 
            print_line(Variant(shot_angle));
    }
    return volley;
}

Vector2 BulletSpawner::_get_shot_direction(const Vector2 &p_position, const Vector2 &p_normal) const{
    switch (aim_mode){
        case RADIAL:
            return p_normal.rotated(aim_angle + get_adjusted_global_rotation());

        case UNIFORM:
            return Vector2(1,0).rotated(aim_angle + get_adjusted_global_rotation());
        
        case TARGET_LOCAL:
            return (target_position - p_position).normalized();

        case TARGET_GLOBAL:
            return (target_position - (get_global_position() + p_position)).normalized();

        default:
            return Vector2();
    }
}

//setters/getters
void BulletSpawner::set_autofire(bool p_enabled) {
    autofire = p_enabled;
    _autofire_time = 0;
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

void BulletSpawner::set_bullet_count(int p_count) {
    bullet_count = p_count;
    _cache_update_required = true;
}

int BulletSpawner::get_bullet_count() const {
    return bullet_count;
}

void BulletSpawner::set_bullet_data(const Ref<BulletData> &p_type) {
    bullet_type = p_type;
    _cache_update_required = true;
}

Ref<BulletData> BulletSpawner::get_bullet_data() const {
    return bullet_type;
}

void BulletSpawner::set_radius(float p_radius) {
    radius = p_radius;
    _cache_update_required = true;    
}

float BulletSpawner::get_radius() const {
    return radius;
}

void BulletSpawner::set_spread(float p_radians){
    spread = p_radians;
    _cache_update_required = true;
}

float BulletSpawner::get_spread() const {
    return spread;
}

void BulletSpawner::set_spread_degrees(float p_degrees){
    spread = p_degrees * M_PI/180.0;
    _cache_update_required = true;
}

float BulletSpawner::get_spread_degrees() const {
    return spread * 180.0/M_PI;
}

void BulletSpawner::set_arc_rotation(float p_radians) {
    arc_rotation = p_radians;
    _cache_update_required = true;
}

float BulletSpawner::get_arc_rotation() const {
    return arc_rotation;
}

void BulletSpawner::set_arc_rotation_degrees(float p_degrees) {
    arc_rotation = p_degrees * M_PI/180.0;
    _cache_update_required = true;
}

float BulletSpawner::get_arc_rotation_degrees() const {
    return arc_rotation * 180.0/M_PI;
}

void BulletSpawner::set_arc_offset(float p_offset) {
    arc_offset = p_offset;
    _cache_update_required = true;
}

float BulletSpawner::get_arc_offset() const {
    return arc_offset;
}

void BulletSpawner::set_aim_mode(AimMode p_mode){
    aim_mode = p_mode;
    _cache_update_required = true;
    _change_notify();
}

BulletSpawner::AimMode BulletSpawner::get_aim_mode() const{
    return aim_mode;
}

void BulletSpawner::set_aim_angle(float p_radians){
    aim_angle = p_radians;
    if (aim_mode == RADIAL || aim_mode == UNIFORM){
        _cache_update_required = true;
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

void BulletSpawner::set_target_position(const Vector2 &p_position) {
    target_position = p_position;
    if (aim_mode == TARGET_LOCAL || aim_mode == TARGET_GLOBAL) {
        _cache_update_required = true;
    }
}

Vector2 BulletSpawner::get_target_position() const {
    return target_position;
}

void BulletSpawner::set_scatter_type(ScatterType p_type) {
    scatter_type = p_type;
    _change_notify();
}

BulletSpawner::ScatterType BulletSpawner::get_scatter_type() const {
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
    _cache_update_required = true;
}

bool BulletSpawner::get_inherit_rotation() const {
    return inherit_rotation;
}

void BulletSpawner::set_self_rotation(float p_radians) {
    self_rotation = p_radians;
    _cache_update_required = true;
}

float BulletSpawner::get_self_rotation() const {
    return self_rotation;
}

void BulletSpawner::set_self_rotation_degrees(float p_degrees) {
    self_rotation = p_degrees * M_PI/180.0;
    _cache_update_required = true;
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
    _cache_update_required = true;
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
    _cache_update_required = true;
}

bool BulletSpawner::get_inherit_scale() const {
    return inherit_scale;
}

void BulletSpawner::set_self_scale(const Vector2 &p_scale) {
    self_scale = p_scale;
    _cache_update_required = true;
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
    _cache_update_required = true;
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

void BulletSpawner::_draw_adjusted_arc(float p_inner_rad, float p_outer_rad, const Vector2 &p_volley_start, int p_point_count, const Color &p_color) {
    
}

void BulletSpawner::_validate_property(PropertyInfo &property) const{
    if (property.name == "scatter_range_degrees" && scatter_type == NONE){
        property.usage = 0;
    }

    if (property.name == "target_position" && !(aim_mode == TARGET_LOCAL || aim_mode == TARGET_GLOBAL)){
        property.usage = PROPERTY_USAGE_NOEDITOR;
    }

    if (property.name == "aim_angle_degrees" && !(aim_mode == RADIAL || aim_mode == UNIFORM)){
        property.usage = PROPERTY_USAGE_NOEDITOR;
    }
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

    ClassDB::bind_method(D_METHOD("set_bullet_count", "count"), &BulletSpawner::set_bullet_count);
    ClassDB::bind_method(D_METHOD("get_bullet_count"), &BulletSpawner::get_bullet_count);

    ClassDB::bind_method(D_METHOD("set_bullet_data", "data"), &BulletSpawner::set_bullet_data);
    ClassDB::bind_method(D_METHOD("get_bullet_data"), &BulletSpawner::get_bullet_data);

    ClassDB::bind_method(D_METHOD("set_radius", "radius"), &BulletSpawner::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &BulletSpawner::get_radius);

    ClassDB::bind_method(D_METHOD("set_spread", "radians"), &BulletSpawner::set_spread);
    ClassDB::bind_method(D_METHOD("get_spread"), &BulletSpawner::get_spread);

    ClassDB::bind_method(D_METHOD("set_spread_degrees", "degrees"), &BulletSpawner::set_spread_degrees);
    ClassDB::bind_method(D_METHOD("get_spread_degrees"), &BulletSpawner::get_spread_degrees);

    ClassDB::bind_method(D_METHOD("set_arc_rotation", "radians"), &BulletSpawner::set_arc_rotation);
    ClassDB::bind_method(D_METHOD("get_arc_rotation"), &BulletSpawner::get_arc_rotation);

    ClassDB::bind_method(D_METHOD("set_arc_rotation_degrees", "degrees"), &BulletSpawner::set_arc_rotation_degrees);
    ClassDB::bind_method(D_METHOD("get_arc_rotation_degrees"), &BulletSpawner::get_arc_rotation_degrees);

    ClassDB::bind_method(D_METHOD("set_arc_offset", "offset"), &BulletSpawner::set_arc_offset);
    ClassDB::bind_method(D_METHOD("get_arc_offset"), &BulletSpawner::get_arc_offset);

    ClassDB::bind_method(D_METHOD("set_aim_mode", "mode"), &BulletSpawner::set_aim_mode);
    ClassDB::bind_method(D_METHOD("get_aim_mode"), &BulletSpawner::get_aim_mode);

    ClassDB::bind_method(D_METHOD("set_aim_angle", "radians"), &BulletSpawner::set_aim_angle);
    ClassDB::bind_method(D_METHOD("get_aim_angle"), &BulletSpawner::get_aim_angle);

    ClassDB::bind_method(D_METHOD("set_aim_angle_degrees", "degrees"), &BulletSpawner::set_aim_angle_degrees);
    ClassDB::bind_method(D_METHOD("get_aim_angle_degrees"), &BulletSpawner::get_aim_angle_degrees);

    ClassDB::bind_method(D_METHOD("set_target_position", "mode"), &BulletSpawner::set_target_position);
    ClassDB::bind_method(D_METHOD("get_target_position"), &BulletSpawner::get_target_position);

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
    ADD_PROPERTY(PropertyInfo(Variant::INT, "bullet_count", PROPERTY_HINT_RANGE, "1,100,1,or_greater"), "set_bullet_count", "get_bullet_count");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_data", PROPERTY_HINT_RESOURCE_TYPE, "BulletData"), "set_bullet_data", "get_bullet_data");
    ADD_GROUP("Spawn Shape", "");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius", PROPERTY_HINT_RANGE, "0,100,0.01,or_greater"), "set_radius", "get_radius");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "spread", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_spread", "get_spread");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "spread_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_spread_degrees", "get_spread_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_rotation", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_arc_rotation", "get_arc_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_arc_rotation_degrees", "get_arc_rotation_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_offset", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_arc_offset", "get_arc_offset");
    ADD_GROUP("Aim", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_mode", PROPERTY_HINT_ENUM, "Radial,Uniform,Local Target,Global Target"), "set_aim_mode", "get_aim_mode");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "aim_angle", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_aim_angle", "get_aim_angle");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "aim_angle_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_aim_angle_degrees", "get_aim_angle_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "target_position"), "set_target_position", "get_target_position");
    ADD_GROUP("Scatter", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scatter_type", PROPERTY_HINT_ENUM, "None,Bullet,Volley"), "set_scatter_type", "get_scatter_type");
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

    BIND_ENUM_CONSTANT(RADIAL);
    BIND_ENUM_CONSTANT(UNIFORM);

    BIND_ENUM_CONSTANT(NONE);
    BIND_ENUM_CONSTANT(BULLET);
    BIND_ENUM_CONSTANT(VOLLEY);
}

//initialiser/terminator
BulletSpawner::BulletSpawner() {
    autofire = false;
    interval_frames = 10;
    bullet_count = 1;
    radius = 0.0;
    spread = 0.0;
    arc_rotation = 0.0;
    arc_offset = 0.0;
    aim_mode = RADIAL;
    aim_angle = 0.0;
    scatter_type = NONE;
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