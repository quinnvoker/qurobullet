#include "bullet_type.h"

void BulletType::set_texture(const Ref<Texture> &p_texture) {
	texture = p_texture;
}

Ref<Texture> BulletType::get_texture() const {
	return texture;
}

void BulletType::set_material(const Ref<Material> &p_material) {
	material = p_material;
}

Ref<Material> BulletType::get_material() const {
	return material;
}

void BulletType::set_face_direction(bool p_enabled) {
	face_direction = p_enabled;
}

bool BulletType::get_face_direction() const {
	return face_direction;
}

void BulletType::set_scale(float p_scale) {
	scale = p_scale;
}

float BulletType::get_scale() const {
	return scale;
}

void BulletType::set_damage(float p_amount) {
	damage = p_amount;
}

float BulletType::get_damage() const {
	return damage;
}

void BulletType::set_collision_shape(const Ref<Shape2D> &p_shape) {
	collision_shape = p_shape;
}

Ref<Shape2D> BulletType::get_collision_shape() const {
	return collision_shape;
}

void BulletType::set_collision_mask(int p_mask) {
	collision_mask = p_mask;
}

int BulletType::get_collision_mask() const {
	return collision_mask;
}

void BulletType::set_speed(float p_speed) {
	speed = p_speed;
}

float BulletType::get_speed() const {
	return speed;
}

void BulletType::set_linear_acceleration(float p_acceleration) {
	linear_acceleration = p_acceleration;
}

float BulletType::get_linear_acceleration() const {
	return linear_acceleration;
}

void BulletType::set_curve_rate(float p_degrees_per_sec) {
	curve_rate = p_degrees_per_sec;
}

float BulletType::get_curve_rate() const {
	return curve_rate;
}

void BulletType::set_sin_amplitude(float p_amplitude) {
	sin_amplitude = p_amplitude;
}

float BulletType::get_sin_amplitude() const {
	return sin_amplitude;
}

void BulletType::set_sin_frequency(float p_freq) {
	sin_frequency = p_freq;
}

float BulletType::get_sin_frequency() const {
	return sin_frequency;
}

void BulletType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &BulletType::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &BulletType::get_texture);

	ClassDB::bind_method(D_METHOD("set_material", "material"), &BulletType::set_material);
	ClassDB::bind_method(D_METHOD("get_material"), &BulletType::get_material);

	ClassDB::bind_method(D_METHOD("set_face_direction", "enabled"), &BulletType::set_face_direction);
	ClassDB::bind_method(D_METHOD("get_face_direction"), &BulletType::get_face_direction);

	ClassDB::bind_method(D_METHOD("set_scale", "scale"), &BulletType::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &BulletType::get_scale);

	ClassDB::bind_method(D_METHOD("set_damage", "damage"), &BulletType::set_damage);
	ClassDB::bind_method(D_METHOD("get_damage"), &BulletType::get_damage);

	ClassDB::bind_method(D_METHOD("set_collision_shape", "collision_shape"), &BulletType::set_collision_shape);
	ClassDB::bind_method(D_METHOD("get_collision_shape"), &BulletType::get_collision_shape);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "collision_mask"), &BulletType::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &BulletType::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletType::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &BulletType::get_speed);

	ClassDB::bind_method(D_METHOD("set_linear_acceleration", "linear_acceleration"), &BulletType::set_linear_acceleration);
	ClassDB::bind_method(D_METHOD("get_linear_acceleration"), &BulletType::get_linear_acceleration);

	ClassDB::bind_method(D_METHOD("set_curve_rate", "curve_rate"), &BulletType::set_curve_rate);
	ClassDB::bind_method(D_METHOD("get_curve_rate"), &BulletType::get_curve_rate);

	ClassDB::bind_method(D_METHOD("set_sin_amplitude", "sin_amplitude"), &BulletType::set_sin_amplitude);
	ClassDB::bind_method(D_METHOD("get_sin_amplitude"), &BulletType::get_sin_amplitude);

	ClassDB::bind_method(D_METHOD("set_sin_frequency", "sin_frequency"), &BulletType::set_sin_frequency);
	ClassDB::bind_method(D_METHOD("get_sin_frequency"), &BulletType::get_sin_frequency);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "face_direction"), "set_face_direction", "get_face_direction");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "damage"), "set_damage", "get_damage");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collision_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_collision_shape", "get_collision_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "speed"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "linear_acceleration"), "set_linear_acceleration", "get_linear_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "curve_rate"), "set_curve_rate", "get_curve_rate");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "sin_amplitude"), "set_sin_amplitude", "get_sin_amplitude");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "sin_frequency"), "set_sin_frequency", "get_sin_frequency");
}

BulletType::BulletType() {
	texture = Ref<Texture>();
	material = Ref<Material>();
	face_direction = false;
	scale = 1.0;
	damage = 0.0;
	collision_shape = Ref<Shape2D>();
	collision_mask = 0;
	speed = 100.0;
	linear_acceleration = 0.0;
	curve_rate = 0.0;
	sin_amplitude = 0.0;
	sin_frequency = 0.0;
}

BulletType::~BulletType() {
	// cleanup goes here
}