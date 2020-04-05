#include "bullet_spawner.h"

//"overrides"
void BulletSpawner::_notification(int p_what) {
    
}

void BulletSpawner::_ready() {
    
}

void BulletSpawner::_process(float delta) {
    
}

void BulletSpawner::_physics_process(float delta){
    if (Engine::get_singleton()->is_editor_hint()){
        return;
    }
    if (autofire){
        autofire_step++;
        if (autofire_step >= interval_frames){
            fire();
            autofire_step = 0;
        }
    }
}

//public functions
void BulletSpawner::fire() {
}

Array BulletSpawner::get_shots() {
    if (shots_update_required){
        _update_shots();
    }
    return shots;
}

Array BulletSpawner::get_scattered_shots() {
    //implement randomizer
    return shots;    
}

//private functions
void BulletSpawner::_update_shots() {
}

Vector2 BulletSpawner::_get_spawn_position(const Vector2 &p_shot_dir) {
    return (p_shot_dir * spawn_radius * get_adjusted_global_scale()).rotated(get_adjusted_global_rotation());
}

void BulletSpawner::_reset_global_transform() {
    set_global_transform(Transform2D(0, get_global_position()));
}

//setters/getters
void BulletSpawner::set_autofire(bool p_enabled) {
    autofire = p_enabled;
    autofire_step = 0;
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

void BulletSpawner::set_bullet_type(const Ref<BulletType> &p_type) {
    bullet_type = p_type;
    shots_update_required = true;
}

Ref<BulletType> BulletSpawner::get_bullet_type() const {
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
    Node2D *parent_2D = dynamic_cast<Node2D*>(parent);
    if (inherit_rotation && parent_2D != NULL){
        self_rotation = p_radians - parent_2D->get_global_rotation();
    } else {
        self_rotation = p_radians;
    }
    shots_update_required = true;
}

float BulletSpawner::get_adjusted_global_rotation() const {
    Node *parent = get_parent();
    Node2D *parent_2D = dynamic_cast<Node2D*>(parent);
    if (inherit_rotation && parent_2D != NULL){
        return self_rotation + parent_2D ->get_global_rotation();
    } else {
        return self_rotation;
    }
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
    Node2D *parent_2D = dynamic_cast<Node2D*>(parent);
    if (inherit_scale && parent_2D != NULL){
        self_scale = p_scale / parent_2D->get_global_scale();
    } else {
        self_scale = p_scale;
    }
    shots_update_required = true;

}

Vector2 BulletSpawner::get_adjusted_global_scale() const {
    Node *parent = get_parent();
    Node2D *parent_2D = dynamic_cast<Node2D*>(parent);
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
    inherit_rotation = false;
    self_rotation = 0.0;
    inherit_scale = false;
    self_scale = Vector2(1,1);
    in_game_preview = false;
    preview_color = Color(0.0, 1.0, 0.0, 1.0); //green
}

BulletSpawner::~BulletSpawner() {
    
}