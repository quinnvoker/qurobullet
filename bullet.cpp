#include "bullet.h"

void Bullet::spawn(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction) {
	time = 0.0;
	rotation = 0.0;
    _popped = false;
	_offset = Vector2(0, 0);
    set_type(p_type);
    set_position(p_position);
    set_direction(p_direction);
	VS::get_singleton()->canvas_item_set_visible(ci_rid, true);
}

void Bullet::update(float delta) {
    float current_speed = type->get_speed() + type->get_linear_acceleration() * time;
	set_direction(direction.rotated(Math::deg2rad(type->get_curve_rate()) * delta));
	position += direction * current_speed * delta;
	_update_offset();
	time += delta;
	if (type->get_lifetime() >= 0.001 && time > type->get_lifetime()){
		pop();
	}
}

void Bullet::pop() {
    _popped = true;
	VS::get_singleton()->canvas_item_set_visible(ci_rid, false);
}

bool Bullet::is_popped() {
    return _popped;
}

bool Bullet::can_collide() {
	return !type.is_null() && (!type->get_collision_shape().is_null() && type->get_collision_mask() != 0);
}

void Bullet::_update_offset(){
	position -= _offset;
	Vector2 h_offset;
	Vector2 v_offset;
	Vector2 perpendicular = direction.rotated(M_PI_2);
	
	switch (type->get_h_wave_type()){
		case BulletType::WaveType::SIN:
			h_offset = direction * type->get_h_wave_amplitude() * sin(time * 2 * M_PI * type->get_h_wave_frequency());
			break;
		
		case BulletType::WaveType::COS:
			h_offset = direction * type->get_h_wave_amplitude() * (cos(time * 2 * M_PI * type->get_h_wave_frequency()) - 1);
			break;

		default:
			h_offset = Vector2();
			break;
	}

	switch (type->get_v_wave_type()){
		case BulletType::WaveType::SIN:
			v_offset = perpendicular * type->get_v_wave_amplitude() * sin(time * 2 * M_PI * type->get_v_wave_frequency());
			break;
		
		case BulletType::WaveType::COS:
			v_offset = perpendicular * type->get_v_wave_amplitude() * (cos(time * 2 * M_PI * type->get_v_wave_frequency()) - 1);
			break;

		default:
			v_offset = Vector2();
			break;
	}

	_offset = h_offset + v_offset;
	position += _offset;
}

void Bullet::set_time(float p_time) {
	time = p_time;    
}

float Bullet::get_time() const {
    return time;
}

void Bullet::set_type(const Ref<BulletType> &p_type) {
	if (!type.is_null()){
		if (p_type->get_material() != type->get_material()){
			ci_set_material(p_type->get_material());
		}
		if (p_type->get_texture() != type->get_texture()){
			ci_draw_texture(p_type->get_texture());
		}
	} else {
		ci_set_material(p_type->get_material());
		ci_draw_texture(p_type->get_texture());
	}
    type = p_type;
}

Ref<BulletType> Bullet::get_type() const {
    return type;
}

void Bullet::set_direction(const Vector2 &p_direction) {
    direction = p_direction;
	if (!type.is_null() && type->get_face_direction()){
		rotation = p_direction.angle();
	}
}

Vector2 Bullet::get_direction() const {
    return direction;
}

void Bullet::set_position(const Vector2 &p_position) {
    position = p_position;
}

Vector2 Bullet::get_position() const {
    return position;
}

void Bullet::set_rotation(float p_radians) {
    rotation = 0.0;
}

float Bullet::get_rotation() const {
    return rotation;
}

Transform2D Bullet::get_transform(){
	Transform2D t;
	t.set_origin(position);
	if (type.is_null()){
		t.set_rotation_and_scale(rotation, Vector2(1,1));
	} else {
		t.set_rotation_and_scale(rotation + type->get_rotation(), type->get_scale());
	}
	return t;
}

void Bullet::set_ci_rid(const RID $p_rid){
	ci_rid = $p_rid;
}

RID Bullet::get_ci_rid() const{
	return ci_rid;
}

void Bullet::ci_set_material(const Ref<Material> &p_material){
	if (p_material.is_null()){
		return;
	}
	VS::get_singleton()->canvas_item_set_material(ci_rid, p_material->get_rid());
}

void Bullet::ci_draw_texture(const Ref<Texture> &p_texture){
	VS::get_singleton()->canvas_item_clear(ci_rid);
	if (p_texture.is_null()){
		return;
	}
	VS::get_singleton()->canvas_item_add_texture_rect(ci_rid, Rect2(-p_texture->get_size() / 2, p_texture->get_size()), p_texture->get_rid());
}

void Bullet::_bind_methods(){
	ClassDB::bind_method(D_METHOD("spawn", "type", "position", "direction"), &Bullet::spawn);

	ClassDB::bind_method(D_METHOD("update", "delta"), &Bullet::update);

	ClassDB::bind_method(D_METHOD("pop"), &Bullet::pop);
	ClassDB::bind_method(D_METHOD("is_popped"), &Bullet::is_popped);

	ClassDB::bind_method(D_METHOD("can_collide"), &Bullet::can_collide);
	
	ClassDB::bind_method(D_METHOD("get_time"), &Bullet::get_time);

	ClassDB::bind_method(D_METHOD("set_type", "type"), &Bullet::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &Bullet::get_type);

	ClassDB::bind_method(D_METHOD("set_direction", "direction"), &Bullet::set_direction);
	ClassDB::bind_method(D_METHOD("get_direction"), &Bullet::get_direction);

	ClassDB::bind_method(D_METHOD("set_position", "position"), &Bullet::set_position);
	ClassDB::bind_method(D_METHOD("get_position"), &Bullet::get_position);

	ClassDB::bind_method(D_METHOD("set_rotation", "rotation"), &Bullet::set_rotation);
	ClassDB::bind_method(D_METHOD("get_rotation"), &Bullet::get_rotation);

	ClassDB::bind_method(D_METHOD("get_transform"), &Bullet::get_transform);

	ClassDB::bind_method(D_METHOD("get_ci_rid"), &Bullet::get_ci_rid);
}

Bullet::Bullet() {
	ci_rid = VS::get_singleton()->canvas_item_create();
    direction = Vector2(0,0);
}

Bullet::~Bullet(){
	VS::get_singleton()->free(ci_rid);
}