#include "bullet_data.h"

void BulletData::set_speed(float p_speed) {
	speed = p_speed;
}

float BulletData::get_speed() const {
	return speed;
}

void BulletData::set_damage(float p_amount) {
	damage = p_amount;
}

float BulletData::get_damage() const {
	return damage;
}

void BulletData::set_lifetime(float p_time) {
	lifetime = p_time;
}

float BulletData::get_lifetime() const {
	return lifetime;
}

void BulletData::set_texture(const Ref<Texture> &p_texture) {
	texture = p_texture;
}

Ref<Texture> BulletData::get_texture() const {
	return texture;
}

void BulletData::set_material(const Ref<Material> &p_material) {
	material = p_material;
}

Ref<Material> BulletData::get_material() const {
	return material;
}

void BulletData::set_collision_shape(const Ref<Shape2D> &p_shape) {
	collision_shape = p_shape;
}

Ref<Shape2D> BulletData::get_collision_shape() const {
	return collision_shape;
}

void BulletData::set_collision_mask(int p_mask) {
	collision_mask = p_mask;
}

int BulletData::get_collision_mask() const {
	return collision_mask;
}

void BulletData::set_linear_acceleration(float p_acceleration) {
	linear_acceleration = p_acceleration;
}

float BulletData::get_linear_acceleration() const {
	return linear_acceleration;
}

void BulletData::set_curve_rate(float p_degrees_per_sec) {
	curve_rate = p_degrees_per_sec;
}

float BulletData::get_curve_rate() const {
	return curve_rate;
}

void BulletData::set_h_wave_type(WaveType p_type) {
	h_wave_type = p_type;
	_change_notify();
}

BulletData::WaveType BulletData::get_h_wave_type() const {
	return h_wave_type;
}

void BulletData::set_h_wave_amplitude(float p_amplitude) {
	h_wave_amplitude = p_amplitude;
}

float BulletData::get_h_wave_amplitude() const {
	return h_wave_amplitude;
}

void BulletData::set_h_wave_frequency(float p_freq) {
	h_wave_frequency = p_freq;
}

float BulletData::get_h_wave_frequency() const {
	return h_wave_frequency;
}

void BulletData::set_v_wave_type(WaveType p_type) {
	v_wave_type = p_type;
	_change_notify();
}

BulletData::WaveType BulletData::get_v_wave_type() const {
	return v_wave_type;
}

void BulletData::set_v_wave_amplitude(float p_amplitude) {
	v_wave_amplitude = p_amplitude;
}

float BulletData::get_v_wave_amplitude() const {
	return v_wave_amplitude;
}

void BulletData::set_v_wave_frequency(float p_freq) {
	v_wave_frequency = p_freq;
}

float BulletData::get_v_wave_frequency() const {
	return v_wave_frequency;
}

void BulletData::set_face_direction(bool p_enabled) {
	face_direction = p_enabled;
}

bool BulletData::get_face_direction() const {
	return face_direction;
}

void BulletData::set_scale(Vector2 p_scale) {
	scale = p_scale;
}

void BulletData::set_rotation(float p_radians){
	rotation = p_radians;
}

float BulletData::get_rotation() const {
	return rotation;
}

void BulletData::set_rotation_degrees(float p_degrees) {
	rotation = Math::deg2rad(p_degrees);
}

float BulletData::get_rotation_degrees() const {
	return Math::rad2deg(rotation);
}

Vector2 BulletData::get_scale() const {
	return scale;
}

void BulletData::_validate_property(PropertyInfo &property) const {
	if ((property.name == "h_wave_amplitude" || property.name == "h_wave_frequency") && h_wave_type == NONE){
		property.usage = PROPERTY_USAGE_STORAGE;
	}
	if ((property.name == "v_wave_amplitude" || property.name == "v_wave_frequency") && v_wave_type == NONE){
		property.usage = PROPERTY_USAGE_STORAGE;
	}
}

void BulletData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletData::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &BulletData::get_speed);

	ClassDB::bind_method(D_METHOD("set_damage", "damage"), &BulletData::set_damage);
	ClassDB::bind_method(D_METHOD("get_damage"), &BulletData::get_damage);

	ClassDB::bind_method(D_METHOD("set_lifetime", "time"), &BulletData::set_lifetime);
	ClassDB::bind_method(D_METHOD("get_lifetime"), &BulletData::get_lifetime);

	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &BulletData::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &BulletData::get_texture);

	ClassDB::bind_method(D_METHOD("set_material", "material"), &BulletData::set_material);
	ClassDB::bind_method(D_METHOD("get_material"), &BulletData::get_material);

	ClassDB::bind_method(D_METHOD("set_collision_shape", "collision_shape"), &BulletData::set_collision_shape);
	ClassDB::bind_method(D_METHOD("get_collision_shape"), &BulletData::get_collision_shape);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "collision_mask"), &BulletData::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &BulletData::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_linear_acceleration", "linear_acceleration"), &BulletData::set_linear_acceleration);
	ClassDB::bind_method(D_METHOD("get_linear_acceleration"), &BulletData::get_linear_acceleration);

	ClassDB::bind_method(D_METHOD("set_curve_rate", "curve_rate"), &BulletData::set_curve_rate);
	ClassDB::bind_method(D_METHOD("get_curve_rate"), &BulletData::get_curve_rate);

	ClassDB::bind_method(D_METHOD("set_h_wave_type", "type"), &BulletData::set_h_wave_type);
	ClassDB::bind_method(D_METHOD("get_h_wave_type"), &BulletData::get_h_wave_type);

	ClassDB::bind_method(D_METHOD("set_h_wave_amplitude", "amplitude"), &BulletData::set_h_wave_amplitude);
	ClassDB::bind_method(D_METHOD("get_h_wave_amplitude"), &BulletData::get_h_wave_amplitude);

	ClassDB::bind_method(D_METHOD("set_h_wave_frequency", "frequency"), &BulletData::set_h_wave_frequency);
	ClassDB::bind_method(D_METHOD("get_h_wave_frequency"), &BulletData::get_h_wave_frequency);

	ClassDB::bind_method(D_METHOD("set_v_wave_type", "type"), &BulletData::set_v_wave_type);
	ClassDB::bind_method(D_METHOD("get_v_wave_type"), &BulletData::get_v_wave_type);

	ClassDB::bind_method(D_METHOD("set_v_wave_amplitude", "amplitude"), &BulletData::set_v_wave_amplitude);
	ClassDB::bind_method(D_METHOD("get_v_wave_amplitude"), &BulletData::get_v_wave_amplitude);

	ClassDB::bind_method(D_METHOD("set_v_wave_frequency", "frequency"), &BulletData::set_v_wave_frequency);
	ClassDB::bind_method(D_METHOD("get_v_wave_frequency"), &BulletData::get_v_wave_frequency);

	ClassDB::bind_method(D_METHOD("set_face_direction", "enabled"), &BulletData::set_face_direction);
	ClassDB::bind_method(D_METHOD("get_face_direction"), &BulletData::get_face_direction);

	ClassDB::bind_method(D_METHOD("set_rotation", "radians"), &BulletData::set_rotation);
	ClassDB::bind_method(D_METHOD("get_rotation"), &BulletData::get_rotation);

	ClassDB::bind_method(D_METHOD("set_rotation_degrees", "degrees"), &BulletData::set_rotation_degrees);
	ClassDB::bind_method(D_METHOD("get_rotation_degrees"), &BulletData::get_rotation_degrees);

	ClassDB::bind_method(D_METHOD("set_scale", "scale"), &BulletData::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &BulletData::get_scale);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "speed", PROPERTY_HINT_RANGE, "0,500,0.01,or_greater"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "damage", PROPERTY_HINT_RANGE, "0,100,1,or_greater"), "set_damage", "get_damage");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lifetime", PROPERTY_HINT_RANGE, "0,30,0.01,or_greater"), "set_lifetime", "get_lifetime");
	ADD_GROUP("Drawing", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");
	ADD_GROUP("Collision", "collision_");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collision_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_collision_shape", "get_collision_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_GROUP("Trajectory", "");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "linear_acceleration", PROPERTY_HINT_RANGE, "-100,100,0.01,or_lesser,or_greater"), "set_linear_acceleration", "get_linear_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "curve_rate", PROPERTY_HINT_RANGE, "-360,360,0.01,or_lesser,or_greater"), "set_curve_rate", "get_curve_rate");
	ADD_GROUP("Parallel Offset Wave", "h_wave_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "h_wave_type", PROPERTY_HINT_ENUM, "None,Sin,Cos"), "set_h_wave_type", "get_h_wave_type");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "h_wave_amplitude", PROPERTY_HINT_RANGE, "-100,100,0.01,or_lesser,or_greater"), "set_h_wave_amplitude", "get_h_wave_amplitude");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "h_wave_frequency", PROPERTY_HINT_RANGE, "0,5,0.001,or_greater"), "set_h_wave_frequency", "get_h_wave_frequency");
	ADD_GROUP("Perpendicular Offset Wave", "v_wave_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "v_wave_type", PROPERTY_HINT_ENUM, "None,Sin,Cos"), "set_v_wave_type", "get_v_wave_type");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "v_wave_amplitude", PROPERTY_HINT_RANGE, "-100,100,0.01,or_lesser,or_greater"), "set_v_wave_amplitude", "get_v_wave_amplitude");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "v_wave_frequency", PROPERTY_HINT_RANGE, "0,5,0.001,or_greater"), "set_v_wave_frequency", "get_v_wave_frequency");
	ADD_GROUP("Transform Modifiers", "");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_rotation", "get_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_rotation_degrees", "get_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "face_direction"), "set_face_direction", "get_face_direction");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");

	BIND_ENUM_CONSTANT(NONE);
    BIND_ENUM_CONSTANT(SIN);
    BIND_ENUM_CONSTANT(COS);
}

BulletData::BulletData() {
	lifetime = 0.0;
	texture = Ref<Texture>();
	material = Ref<Material>();
	face_direction = false;
	scale = Vector2(1,1);
	damage = 0.0;
	collision_shape = Ref<Shape2D>();
	collision_mask = 0;
	speed = 100.0;
	linear_acceleration = 0.0;
	curve_rate = 0.0;
	v_wave_amplitude = 0.0;
	v_wave_frequency = 0.0;
}

BulletData::~BulletData() {
	// cleanup goes here
}