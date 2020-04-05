#include "bullet_spawner.h"

//"overrides"
void BulletSpawner::_notification(int p_what) {
    
}

void BulletSpawner::_ready() {
    
}

void BulletSpawner::_process(float delta) {
    
}

void BulletSpawner::_physics_process(float delta){
    
}

//public functions
void BulletSpawner::fire() {
    
}

Array BulletSpawner::get_shots() {
    
}

Array BulletSpawner::get_scattered_shots() {
    
}

//private functions
Array BulletSpawner::_update_shots() {
    
}

Vector2 BulletSpawner::_get_spawn_position(const Vector2 &p_shot_dir) {
    
}

void BulletSpawner::_reset_global_transform() {
    
}

//setters/getters
void BulletSpawner::set_autofire(bool p_enabled) {
    
}

bool BulletSpawner::get_autofire() const {
    
}

void BulletSpawner::set_interval_frames(int p_interval) {
    
}

int BulletSpawner::get_interval_frames() const {
    
}

void BulletSpawner::set_bullet_type(const Ref<BulletType> &p_type) {
    
}

Ref<BulletType> BulletSpawner::get_bullet_type() const {
    
}

void BulletSpawner::set_spawn_radius(float p_radius) {
    
}

float BulletSpawner::get_spawn_radius() const {
    
}

void BulletSpawner::set_spawn_angle(float p_radians) {
    
}

float BulletSpawner::get_spawn_angle() const {
    
}

void BulletSpawner::set_spawn_angle_degrees(float p_degrees) {
    
}

float BulletSpawner::get_spawn_angle_degrees() const {
    
}

void BulletSpawner::set_bullet_count(int p_count) {
    
}

int BulletSpawner::get_bullet_count() const {
    
}

void BulletSpawner::set_scatter_type(int p_type) {
    
}

int BulletSpawner::get_scatter_type() const {
    
}

void BulletSpawner::set_scatter_range(float p_radians) {
    
}

float BulletSpawner::get_scatter_range() const {
    
}

void BulletSpawner::set_scatter_range_degrees(float p_degrees) {
    
}

float BulletSpawner::get_scatter_range_degrees() const {
    
}

void BulletSpawner::set_self_rotation(float p_radians) {
    
}

float BulletSpawner::get_self_rotation() const {
    
}

void BulletSpawner::set_self_rotation_degrees(float p_degrees) {
    
}

float BulletSpawner::get_self_rotation_degrees() const {
    
}

void BulletSpawner::set_adjusted_global_rotation(float p_radians) {
    
}

float BulletSpawner::get_adjusted_global_rotation() const {
    
}

void BulletSpawner::set_self_scale(const Vector2 &p_scale) {
    
}

float BulletSpawner::get_self_scale() const {
    
}

void BulletSpawner::set_adjusted_global_scale(const Vector2 &p_scale) {
    
}

float BulletSpawner::get_adjusted_global_scale() const {
    
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
    
}

BulletSpawner::~BulletSpawner() {
    
}