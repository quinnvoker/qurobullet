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
    emit_signal("volley_fired", bullet_type->duplicate(), get_global_position(), get_scattered_volley());
}

Array BulletSpawner::get_volley() {
    if (_cache_update_required){
        _update_cached_volley();
    }
    return _cached_volley;
}

Array BulletSpawner::get_scattered_volley() {
    if (scatter_mode == NONE || scatter_range <= 0.0){
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
void BulletSpawner::_update_cached_volley() {
    Array new_volley = _create_volley();
    _cached_volley = new_volley;
    _cache_update_required = false;
}

Array BulletSpawner::_create_volley() const {
    Array volley;
    if (bullet_count == 1 || (arc_width == 0.0 && !(scatter_mode == BULLET && radius > 0))){
        Vector2 dir = Vector2(1,0).rotated(arc_rotation + get_adjusted_global_rotation());
        Dictionary shot;
        shot["position"] = dir * radius;
        shot["direction"] = _get_shot_direction(dir * radius, dir);
        volley.push_back(shot);
        return volley;
    }

    float arc_extent = arc_width / 2;
    float spacing = arc_width / (bullet_count - 1);
    bool spacing_maxed = false;

    float volley_start = -arc_extent;
    if (spacing > 2 * M_PI / bullet_count){
        spacing = 2 * M_PI / bullet_count;
        volley_start = -M_PI + spacing / 2;
        spacing_maxed = true;
    }

    for (int i = 0; i < bullet_count; i++){
        float shot_angle = spacing * i + (arc_offset * arc_width / 2);
        if (!spacing_maxed){
            shot_angle = Math::wrapf(shot_angle, 0 - spacing / 2, arc_width + spacing / 2);
        }
        shot_angle += volley_start;
        if (arc_width >= 2 * M_PI || Math::abs(Math::fmod(shot_angle, float(M_PI))) <= arc_extent + 0.001){
            Dictionary shot;
            Vector2 shot_normal = Vector2(1,0).rotated(shot_angle + arc_rotation);
            shot["position"] = _get_shot_position(shot_normal);
            shot["direction"] = _get_shot_direction(shot["position"], shot_normal);
            volley.push_back(shot);
        }
    }
    return volley;
}

Vector2 BulletSpawner::_get_shot_position(const Vector2 &p_normal) const{
    return (p_normal * radius * get_adjusted_global_scale()).rotated(get_adjusted_global_rotation());
}

Vector2 BulletSpawner::_get_shot_direction(const Vector2 &p_position, const Vector2 &p_normal) const{
    switch (aim_mode){
        case RADIAL:
            return p_normal.rotated(aim_angle + get_adjusted_global_rotation());

        case UNIFORM:
            return Vector2(1,0).rotated(aim_angle + get_adjusted_global_rotation());
        
        case TARGET_LOCAL:
            return (aim_target_position - p_position).normalized();

        case TARGET_GLOBAL:
            return (aim_target_position - (get_global_position() + p_position)).normalized();

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

void BulletSpawner::set_bullet_type(const Ref<BulletType> &p_type) {
    bullet_type = p_type;
    _cache_update_required = true;
}

Ref<BulletType> BulletSpawner::get_bullet_type() const {
    return bullet_type;
}

void BulletSpawner::set_radius(float p_radius) {
    radius = p_radius;
    _cache_update_required = true;    
}

float BulletSpawner::get_radius() const {
    return radius;
}

void BulletSpawner::set_arc_width(float p_radians){
    arc_width = p_radians;
    _cache_update_required = true;
}

float BulletSpawner::get_arc_width() const {
    return arc_width;
}

void BulletSpawner::set_arc_width_degrees(float p_degrees){
    arc_width = Math::deg2rad(p_degrees);
    _cache_update_required = true;
}

float BulletSpawner::get_arc_width_degrees() const {
    return Math::rad2deg(arc_width);
}

void BulletSpawner::set_arc_rotation(float p_radians) {
    arc_rotation = p_radians;
    _cache_update_required = true;
}

float BulletSpawner::get_arc_rotation() const {
    return arc_rotation;
}

void BulletSpawner::set_arc_rotation_degrees(float p_degrees) {
    arc_rotation = Math::deg2rad(p_degrees);
    _cache_update_required = true;
}

float BulletSpawner::get_arc_rotation_degrees() const {
    return Math::rad2deg(arc_rotation);
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

void BulletSpawner::set_aim_target_position(const Vector2 &p_position) {
    aim_target_position = p_position;
    if (aim_mode == TARGET_LOCAL || aim_mode == TARGET_GLOBAL) {
        _cache_update_required = true;
    }
}

Vector2 BulletSpawner::get_aim_target_position() const {
    return aim_target_position;
}

void BulletSpawner::set_scatter_mode(ScatterMode p_type) {
    scatter_mode = p_type;
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

void BulletSpawner::set_inherit_rotation(bool p_enabled){
    inherit_rotation = p_enabled;
    _cache_update_required = true;
}

bool BulletSpawner::get_inherit_rotation() const {
    return inherit_rotation;
}

void BulletSpawner::set_rotation_modifier(float p_radians) {
    rotation_modifier = p_radians;
    _cache_update_required = true;
}

float BulletSpawner::get_rotation_modifier() const {
    return rotation_modifier;
}

void BulletSpawner::set_rotation_modifier_degrees(float p_degrees) {
    rotation_modifier = Math::deg2rad(p_degrees);
    _cache_update_required = true;
}

float BulletSpawner::get_rotation_modifier_degrees() const {
    return Math::rad2deg(rotation_modifier);
}

void BulletSpawner::set_adjusted_global_rotation(float p_radians) {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_rotation && parent_2D != NULL){
        rotation_modifier = p_radians - parent_2D->get_global_rotation();
    } else {
        rotation_modifier = p_radians;
    }
    _cache_update_required = true;
}

float BulletSpawner::get_adjusted_global_rotation() const {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_rotation && parent_2D != NULL){
        return rotation_modifier + parent_2D->get_global_rotation();
    } else {
        return rotation_modifier;
    }
}

void BulletSpawner::set_inherit_scale(bool p_enabled){
    inherit_scale = p_enabled;
    _cache_update_required = true;
}

bool BulletSpawner::get_inherit_scale() const {
    return inherit_scale;
}

void BulletSpawner::set_scale_mod(const Vector2 &p_scale) {
    scale_modifier = p_scale;
    _cache_update_required = true;
}

Vector2 BulletSpawner::get_scale_mod() const {
    return scale_modifier;    
}

void BulletSpawner::set_adjusted_global_scale(const Vector2 &p_scale) {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_scale && parent_2D != NULL){
        scale_modifier = p_scale / parent_2D->get_global_scale();
    } else {
        scale_modifier = p_scale;
    }
    _cache_update_required = true;
}

Vector2 BulletSpawner::get_adjusted_global_scale() const {
    Node *parent = get_parent();
    Node2D *parent_2D = Object::cast_to<Node2D>(parent);
    if (inherit_scale && parent_2D != NULL){
        return scale_modifier * parent_2D->get_global_scale();
    } else {
        return scale_modifier;
    }
}

//drawing functions
void BulletSpawner::_draw_shot_preview(const Color &p_border_col, const Color &p_shot_col) {
    
}

void BulletSpawner::_draw_adjusted_arc(float p_inner_rad, float p_outer_rad, const Vector2 &p_volley_start, int p_point_count, const Color &p_color) {
    
}

void BulletSpawner::_validate_property(PropertyInfo &property) const{
    if (property.name == "scatter_range_degrees" && scatter_mode == NONE){
        property.usage = 0;
    }

    if (property.name == "aim_target_position" && !(aim_mode == TARGET_LOCAL || aim_mode == TARGET_GLOBAL)){
        property.usage = PROPERTY_USAGE_NOEDITOR;
    }

    if (property.name == "aim_angle_degrees" && !(aim_mode == RADIAL || aim_mode == UNIFORM)){
        property.usage = PROPERTY_USAGE_NOEDITOR;
    }
}

//godot binds
void BulletSpawner::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fire"), &BulletSpawner::fire);
    ClassDB::bind_method(D_METHOD("get_volley"), &BulletSpawner::get_volley);
    ClassDB::bind_method(D_METHOD("get_scattered_volley"), &BulletSpawner::get_scattered_volley);

    ClassDB::bind_method(D_METHOD("set_autofire", "enabled"), &BulletSpawner::set_autofire);
    ClassDB::bind_method(D_METHOD("get_autofire"), &BulletSpawner::get_autofire);

    ClassDB::bind_method(D_METHOD("set_interval_frames", "interval"), &BulletSpawner::set_interval_frames);
    ClassDB::bind_method(D_METHOD("get_interval_frames"), &BulletSpawner::get_interval_frames);

    ClassDB::bind_method(D_METHOD("set_bullet_count", "count"), &BulletSpawner::set_bullet_count);
    ClassDB::bind_method(D_METHOD("get_bullet_count"), &BulletSpawner::get_bullet_count);

    ClassDB::bind_method(D_METHOD("set_bullet_type", "type"), &BulletSpawner::set_bullet_type);
    ClassDB::bind_method(D_METHOD("get_bullet_type"), &BulletSpawner::get_bullet_type);

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

    ClassDB::bind_method(D_METHOD("set_inherit_rotation", "enabled"), &BulletSpawner::set_inherit_rotation);
    ClassDB::bind_method(D_METHOD("get_inherit_rotation"), &BulletSpawner::get_inherit_rotation);

    ClassDB::bind_method(D_METHOD("set_rotation_modifier", "radians"), &BulletSpawner::set_rotation_modifier);
    ClassDB::bind_method(D_METHOD("get_rotation_modifier"), &BulletSpawner::get_rotation_modifier);

    ClassDB::bind_method(D_METHOD("set_rotation_modifier_degrees", "degrees"), &BulletSpawner::set_rotation_modifier_degrees);
    ClassDB::bind_method(D_METHOD("get_rotation_modifier_degrees"), &BulletSpawner::get_rotation_modifier_degrees);

    ClassDB::bind_method(D_METHOD("set_adjusted_global_rotation", "radians"), &BulletSpawner::set_adjusted_global_rotation);
    ClassDB::bind_method(D_METHOD("get_adjusted_global_rotation"), &BulletSpawner::get_adjusted_global_rotation);

    ClassDB::bind_method(D_METHOD("set_inherit_scale", "enabled"), &BulletSpawner::set_inherit_scale);
    ClassDB::bind_method(D_METHOD("get_inherit_scale"), &BulletSpawner::get_inherit_scale);

    ClassDB::bind_method(D_METHOD("set_scale_mod", "scale"), &BulletSpawner::set_scale_mod);
    ClassDB::bind_method(D_METHOD("get_scale_mod"), &BulletSpawner::get_scale_mod);

    ClassDB::bind_method(D_METHOD("set_adjusted_global_scale", "scale"), &BulletSpawner::set_adjusted_global_scale);
    ClassDB::bind_method(D_METHOD("get_adjusted_global_scale"), &BulletSpawner::get_adjusted_global_scale);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autofire"), "set_autofire", "get_autofire");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "interval_frames", PROPERTY_HINT_RANGE, "1,300,1,or_greater"), "set_interval_frames", "get_interval_frames");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "bullet_count", PROPERTY_HINT_RANGE, "1,100,1,or_greater"), "set_bullet_count", "get_bullet_count");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), "set_bullet_type", "get_bullet_type");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius", PROPERTY_HINT_RANGE, "0,100,0.01,or_greater"), "set_radius", "get_radius");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_width", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_arc_width", "get_arc_width");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_width_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_arc_width_degrees", "get_arc_width_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_rotation", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_arc_rotation", "get_arc_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_arc_rotation_degrees", "get_arc_rotation_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "arc_offset", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_arc_offset", "get_arc_offset");
    ADD_GROUP("Aim", "aim_");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_mode", PROPERTY_HINT_ENUM, "Radial,Uniform,Local Target,Global Target"), "set_aim_mode", "get_aim_mode");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "aim_angle", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_aim_angle", "get_aim_angle");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "aim_angle_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_aim_angle_degrees", "get_aim_angle_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "aim_target_position"), "set_aim_target_position", "get_aim_target_position");
    ADD_GROUP("Scatter", "scatter_");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scatter_mode", PROPERTY_HINT_ENUM, "None,Bullet,Volley"), "set_scatter_mode", "get_scatter_mode");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "scatter_range", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_scatter_range", "get_scatter_range");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "scatter_range_degrees", PROPERTY_HINT_RANGE, "0,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_scatter_range_degrees", "get_scatter_range_degrees");
    ADD_GROUP("Transform Modifiers", "");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "inherit_rotation"), "set_inherit_rotation", "get_inherit_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation_modifier", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_rotation_modifier", "get_rotation_modifier");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation_modifier_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_rotation_modifier_degrees", "get_rotation_modifier_degrees");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "inherit_scale"), "set_inherit_scale", "get_inherit_scale");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale_modifier"), "set_scale_mod", "get_scale_mod");

    ADD_SIGNAL(MethodInfo("bullet_fired", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::VECTOR2, "direction")));
    ADD_SIGNAL(MethodInfo("volley_fired", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::ARRAY, "volley")));

    BIND_ENUM_CONSTANT(RADIAL);
    BIND_ENUM_CONSTANT(UNIFORM);
    BIND_ENUM_CONSTANT(TARGET_LOCAL);
    BIND_ENUM_CONSTANT(TARGET_GLOBAL);

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
    arc_width = 0.0;
    arc_rotation = 0.0;
    arc_offset = 0.0;
    aim_mode = RADIAL;
    aim_angle = 0.0;
    scatter_mode = NONE;
    scatter_range = 0.0;
    inherit_rotation = true;
    rotation_modifier = 0.0;
    inherit_scale = true;
    scale_modifier = Vector2(1,1);
    in_game_preview = false;
    preview_color = Color(0.0, 1.0, 0.0, 1.0); //green
}

BulletSpawner::~BulletSpawner() {
    
}