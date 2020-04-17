#include "bullet.h"

void Bullet::spawn(const Ref<BulletData> &p_data, const Vector2 &p_position, const Vector2 &p_direction) {
	time = 0.0;
	rotation = 0.0;
    _popped = false;
	_offset = Vector2(0, 0);
    set_data(p_data);
    set_position(p_position);
    set_direction(p_direction);
	VS::get_singleton()->canvas_item_set_visible(ci_rid, true);
}

void Bullet::update(float delta) {
    float current_speed = data->get_speed() + data->get_linear_acceleration() * time;
	set_direction(direction.rotated(Math::deg2rad(data->get_curve_rate()) * delta));
	position += direction * current_speed * delta;
	_update_offset();
	time += delta;
	if (data->get_lifetime() >= 0.001 && time > data->get_lifetime()){
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
	return !data.is_null() && (!data->get_collision_shape().is_null() && data->get_collision_mask() != 0);
}

void Bullet::_update_offset(){
	position -= _offset;
	Vector2 h_offset;
	Vector2 v_offset;
	Vector2 perpendicular = direction.rotated(M_PI_2);
	
	switch (data->get_h_wave_type()){
		case BulletData::WaveType::SIN:
			h_offset = direction * data->get_h_wave_amplitude() * sin(time * 2 * M_PI * data->get_h_wave_frequency());
			break;
		
		case BulletData::WaveType::COS:
			h_offset = direction * data->get_h_wave_amplitude() * (cos(time * 2 * M_PI * data->get_h_wave_frequency()) - 1);
			break;

		default:
			h_offset = Vector2();
			break;
	}

	switch (data->get_v_wave_type()){
		case BulletData::WaveType::SIN:
			v_offset = perpendicular * data->get_v_wave_amplitude() * sin(time * 2 * M_PI * data->get_v_wave_frequency());
			break;
		
		case BulletData::WaveType::COS:
			v_offset = perpendicular * data->get_v_wave_amplitude() * (cos(time * 2 * M_PI * data->get_v_wave_frequency()) - 1);
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

void Bullet::set_data(const Ref<BulletData> &p_data) {
	if (!data.is_null()){
		if (p_data->get_material() != data->get_material()){
			ci_set_material(p_data->get_material());
		}
		if (p_data->get_texture() != data->get_texture()){
			ci_draw_texture(p_data->get_texture());
		}
	} else {
		ci_set_material(p_data->get_material());
		ci_draw_texture(p_data->get_texture());
	}
    data = p_data;
}

Ref<BulletData> Bullet::get_data() const {
    return data;
}

void Bullet::set_direction(const Vector2 &p_direction) {
    direction = p_direction;
	if (!data.is_null() && data->get_face_direction()){
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
	if (data.is_null()){
		t.set_rotation_and_scale(rotation, Vector2(1,1));
	} else {
		t.set_rotation_and_scale(rotation + data->get_rotation(), data->get_scale());
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
	ClassDB::bind_method(D_METHOD("spawn", "data", "position", "direction"), &Bullet::spawn);

	ClassDB::bind_method(D_METHOD("update", "delta"), &Bullet::update);

	ClassDB::bind_method(D_METHOD("pop"), &Bullet::pop);
	ClassDB::bind_method(D_METHOD("is_popped"), &Bullet::is_popped);

	ClassDB::bind_method(D_METHOD("can_collide"), &Bullet::can_collide);
	
	ClassDB::bind_method(D_METHOD("get_time"), &Bullet::get_time);

	ClassDB::bind_method(D_METHOD("set_data", "data"), &Bullet::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &Bullet::get_data);

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