#include "bullet_type.h"

void BulletType::set_speed(float p_speed) {
	speed = p_speed;
}

float BulletType::get_speed() const {
	return speed;
}

void BulletType::set_damage(float p_amount) {
	damage = p_amount;
}

float BulletType::get_damage() const {
	return damage;
}

void BulletType::set_lifetime(float p_time) {
	lifetime = p_time;
}

float BulletType::get_lifetime() const {
	return lifetime;
}

void BulletType::set_texture(const Ref<Texture> &p_texture) {
	texture = p_texture;
}

Ref<Texture> BulletType::get_texture() const {
	return texture;
}

void BulletType::set_modulate(const Color &p_color) {
	modulate = p_color;
}

Color BulletType::get_modulate() const {
	return modulate;
}

void BulletType::set_light_mask(int p_mask) {
	light_mask = p_mask;
}

int BulletType::get_light_mask() const {
	return light_mask;
}

void BulletType::set_material(const Ref<Material> &p_material) {
	material = p_material;
}

Ref<Material> BulletType::get_material() const {
	return material;
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

void BulletType::set_collision_detect_bodies(bool p_enabled) {
	collision_detect_bodies = p_enabled;
}

bool BulletType::get_collision_detect_bodies() const {
	return collision_detect_bodies;
}

void BulletType::set_collision_detect_areas(bool p_enabled) {
	collision_detect_areas = p_enabled;
}

bool BulletType::get_collision_detect_areas() const {
	return collision_detect_areas;
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

void BulletType::set_h_wave_type(WaveType p_type) {
	h_wave_type = p_type;
	_change_notify();
}

BulletType::WaveType BulletType::get_h_wave_type() const {
	return h_wave_type;
}

void BulletType::set_h_wave_amplitude(float p_amplitude) {
	h_wave_amplitude = p_amplitude;
}

float BulletType::get_h_wave_amplitude() const {
	return h_wave_amplitude;
}

void BulletType::set_h_wave_frequency(float p_freq) {
	h_wave_frequency = p_freq;
}

float BulletType::get_h_wave_frequency() const {
	return h_wave_frequency;
}

void BulletType::set_v_wave_type(WaveType p_type) {
	v_wave_type = p_type;
	_change_notify();
}

BulletType::WaveType BulletType::get_v_wave_type() const {
	return v_wave_type;
}

void BulletType::set_v_wave_amplitude(float p_amplitude) {
	v_wave_amplitude = p_amplitude;
}

float BulletType::get_v_wave_amplitude() const {
	return v_wave_amplitude;
}

void BulletType::set_v_wave_frequency(float p_freq) {
	v_wave_frequency = p_freq;
}

float BulletType::get_v_wave_frequency() const {
	return v_wave_frequency;
}

void BulletType::set_face_direction(bool p_enabled) {
	face_direction = p_enabled;
}

bool BulletType::get_face_direction() const {
	return face_direction;
}

void BulletType::set_rotation(float p_radians){
	rotation = p_radians;
}

float BulletType::get_rotation() const {
	return rotation;
}

void BulletType::set_rotation_degrees(float p_degrees) {
	rotation = Math::deg2rad(p_degrees);
}

float BulletType::get_rotation_degrees() const {
	return Math::rad2deg(rotation);
}

void BulletType::set_scale(Vector2 p_scale) {
	scale = p_scale;
}

Vector2 BulletType::get_scale() const {
	return scale;
}

void BulletType::set_custom_data(const Dictionary &p_data){
	custom_data = p_data;
}

Dictionary BulletType::get_custom_data() const {
	return custom_data;
}

void BulletType::_validate_property(PropertyInfo &property) const {
	if ((property.name == "h_wave_amplitude" || property.name == "h_wave_frequency") && h_wave_type == NONE){
		property.usage = PROPERTY_USAGE_STORAGE;
	}
	if ((property.name == "v_wave_amplitude" || property.name == "v_wave_frequency") && v_wave_type == NONE){
		property.usage = PROPERTY_USAGE_STORAGE;
	}
}

void BulletType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletType::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &BulletType::get_speed);

	ClassDB::bind_method(D_METHOD("set_damage", "damage"), &BulletType::set_damage);
	ClassDB::bind_method(D_METHOD("get_damage"), &BulletType::get_damage);

	ClassDB::bind_method(D_METHOD("set_lifetime", "time"), &BulletType::set_lifetime);
	ClassDB::bind_method(D_METHOD("get_lifetime"), &BulletType::get_lifetime);

	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &BulletType::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &BulletType::get_texture);

	ClassDB::bind_method(D_METHOD("set_modulate", "modulate"), &BulletType::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &BulletType::get_modulate);

	ClassDB::bind_method(D_METHOD("set_light_mask", "light_mask"), &BulletType::set_light_mask);
	ClassDB::bind_method(D_METHOD("get_light_mask"), &BulletType::get_light_mask);

	ClassDB::bind_method(D_METHOD("set_material", "material"), &BulletType::set_material);
	ClassDB::bind_method(D_METHOD("get_material"), &BulletType::get_material);

	ClassDB::bind_method(D_METHOD("set_collision_shape", "collision_shape"), &BulletType::set_collision_shape);
	ClassDB::bind_method(D_METHOD("get_collision_shape"), &BulletType::get_collision_shape);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "collision_mask"), &BulletType::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &BulletType::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_collision_detect_bodies", "enabled"), &BulletType::set_collision_detect_bodies);
	ClassDB::bind_method(D_METHOD("get_collision_detect_bodies"), &BulletType::get_collision_detect_bodies);

	ClassDB::bind_method(D_METHOD("set_collision_detect_areas", "enabled"), &BulletType::set_collision_detect_areas);
	ClassDB::bind_method(D_METHOD("get_collision_detect_areas"), &BulletType::get_collision_detect_areas);

	ClassDB::bind_method(D_METHOD("set_linear_acceleration", "linear_acceleration"), &BulletType::set_linear_acceleration);
	ClassDB::bind_method(D_METHOD("get_linear_acceleration"), &BulletType::get_linear_acceleration);

	ClassDB::bind_method(D_METHOD("set_curve_rate", "curve_rate"), &BulletType::set_curve_rate);
	ClassDB::bind_method(D_METHOD("get_curve_rate"), &BulletType::get_curve_rate);

	ClassDB::bind_method(D_METHOD("set_h_wave_type", "type"), &BulletType::set_h_wave_type);
	ClassDB::bind_method(D_METHOD("get_h_wave_type"), &BulletType::get_h_wave_type);

	ClassDB::bind_method(D_METHOD("set_h_wave_amplitude", "amplitude"), &BulletType::set_h_wave_amplitude);
	ClassDB::bind_method(D_METHOD("get_h_wave_amplitude"), &BulletType::get_h_wave_amplitude);

	ClassDB::bind_method(D_METHOD("set_h_wave_frequency", "frequency"), &BulletType::set_h_wave_frequency);
	ClassDB::bind_method(D_METHOD("get_h_wave_frequency"), &BulletType::get_h_wave_frequency);

	ClassDB::bind_method(D_METHOD("set_v_wave_type", "type"), &BulletType::set_v_wave_type);
	ClassDB::bind_method(D_METHOD("get_v_wave_type"), &BulletType::get_v_wave_type);

	ClassDB::bind_method(D_METHOD("set_v_wave_amplitude", "amplitude"), &BulletType::set_v_wave_amplitude);
	ClassDB::bind_method(D_METHOD("get_v_wave_amplitude"), &BulletType::get_v_wave_amplitude);

	ClassDB::bind_method(D_METHOD("set_v_wave_frequency", "frequency"), &BulletType::set_v_wave_frequency);
	ClassDB::bind_method(D_METHOD("get_v_wave_frequency"), &BulletType::get_v_wave_frequency);

	ClassDB::bind_method(D_METHOD("set_face_direction", "enabled"), &BulletType::set_face_direction);
	ClassDB::bind_method(D_METHOD("get_face_direction"), &BulletType::get_face_direction);

	ClassDB::bind_method(D_METHOD("set_rotation", "radians"), &BulletType::set_rotation);
	ClassDB::bind_method(D_METHOD("get_rotation"), &BulletType::get_rotation);

	ClassDB::bind_method(D_METHOD("set_rotation_degrees", "degrees"), &BulletType::set_rotation_degrees);
	ClassDB::bind_method(D_METHOD("get_rotation_degrees"), &BulletType::get_rotation_degrees);

	ClassDB::bind_method(D_METHOD("set_scale", "scale"), &BulletType::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &BulletType::get_scale);

	ClassDB::bind_method(D_METHOD("set_custom_data", "custom_data"), &BulletType::set_custom_data);
	ClassDB::bind_method(D_METHOD("get_custom_data"), &BulletType::get_custom_data);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "speed", PROPERTY_HINT_RANGE, "0,500,0.01,or_greater"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "damage", PROPERTY_HINT_RANGE, "0,100,1,or_greater"), "set_damage", "get_damage");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lifetime", PROPERTY_HINT_RANGE, "0,30,0.01,or_greater"), "set_lifetime", "get_lifetime");
	ADD_GROUP("Appearance", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_mask", PROPERTY_HINT_LAYERS_2D_RENDER), "set_light_mask", "get_light_mask");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");
	ADD_GROUP("Collision", "collision_");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collision_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_collision_shape", "get_collision_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collision_detect_bodies"), "set_collision_detect_bodies", "get_collision_detect_bodies");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collision_detect_areas"), "set_collision_detect_areas", "get_collision_detect_areas");
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
	ADD_GROUP("Transform", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "face_direction"), "set_face_direction", "get_face_direction");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation", PROPERTY_HINT_RANGE, "", PROPERTY_USAGE_NOEDITOR), "set_rotation", "get_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater", PROPERTY_USAGE_EDITOR), "set_rotation_degrees", "get_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");
	ADD_GROUP("Custom Data", "");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "custom_data"), "set_custom_data", "get_custom_data");

	BIND_ENUM_CONSTANT(NONE);
  BIND_ENUM_CONSTANT(SIN);
  BIND_ENUM_CONSTANT(COS);
}

BulletType::BulletType() {
	lifetime = 0.0;
	texture = Ref<Texture>();
	modulate = Color(1,1,1,1);
	light_mask = 1;
	material = Ref<Material>();
	face_direction = false;
	scale = Vector2(1,1);
	damage = 0.0;
	collision_shape = Ref<Shape2D>();
	collision_mask = 1;
	collision_detect_bodies = true;
	collision_detect_areas = true;
	speed = 100.0;
	linear_acceleration = 0.0;
	curve_rate = 0.0;
	v_wave_type = WaveType::NONE;
	v_wave_amplitude = 0.0;
	v_wave_frequency = 0.0;
	rotation = 0.0;
	h_wave_type = WaveType::NONE;
	h_wave_amplitude = 0.0;
	h_wave_frequency = 0.0;
}

BulletType::~BulletType() {
	// cleanup goes here
}
