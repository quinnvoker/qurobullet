#include "bullet.h"

#include "core/os/os.h"
#include "scene/resources/world_2d.h"

void Bullet::_ready() {
	if (get_tree()->is_node_being_edited(this))
		return;
	set_active(false);
}

void Bullet::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			if (type.is_null() or type->get_texture().is_null())
				return;
			Ref<Texture> tex = type->get_texture();
			RID ci = get_canvas_item();

			tex->draw(ci, -tex->get_size() / 2);
		}

		break;

		case NOTIFICATION_READY: {
			_ready();
		}

		default:
			break;
	}
}

void Bullet::spawn(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction){
	set_type(p_type);
	set_position(p_position);
	set_direction(p_direction);
	set_active(true);
	lifetime = 0.0;
	perp_offset = Vector2(0, 0);
}

void Bullet::update_position(float delta) {
	float current_speed = type->get_speed() + type->get_linear_acceleration() * lifetime;
	direction = direction.rotated(type->get_curve_rate() * delta * Math_PI / 180);
	Vector2 perpendicular = direction.rotated(90 * Math_PI / 180);
	float sin_point = sin(lifetime * type->get_sin_frequency());
	Vector2 new_perp_offset = perpendicular * sin_point * type->get_sin_amplitude();
	set_position(get_position() - perp_offset + direction * current_speed * delta + new_perp_offset);
	perp_offset = new_perp_offset;
	lifetime += delta;
}

void Bullet::set_active(bool p_active) {
	active = p_active;
	set_visible(active);
}

bool Bullet::get_active() const {
	return active;
}

void Bullet::set_lifetime(float p_time) {
	lifetime = p_time;
}

float Bullet::get_lifetime() const {
	return lifetime;
}

void Bullet::set_direction(const Vector2 &p_direction) {
	direction = p_direction;
	if (get_tree()->is_node_being_edited(this))
		return;
	if (!type.is_null() && type->get_face_direction())
		set_rotation(direction.angle());
	else
		set_rotation(0.0);
}

Vector2 Bullet::get_direction() const {
	return direction;
}

void Bullet::set_type(const Ref<BulletType> &p_type) {
	type = p_type;
	set_scale(Vector2(type->get_scale(), type->get_scale()));
	set_material(type->get_material());
	update();
}

Ref<BulletType> Bullet::get_type() const {
	return type;
}

void Bullet::_bind_methods() {
	ClassDB::bind_method(D_METHOD("spawn", "type", "position", "direction"), &Bullet::spawn);
	ClassDB::bind_method(D_METHOD("update_position", "delta"), &Bullet::update_position);

	ClassDB::bind_method(D_METHOD("set_active", "active"), &Bullet::set_active);
	ClassDB::bind_method(D_METHOD("get_active"), &Bullet::get_active);

	ClassDB::bind_method(D_METHOD("set_lifetime", "time"), &Bullet::set_lifetime);
	ClassDB::bind_method(D_METHOD("get_lifetime"), &Bullet::get_lifetime);

	ClassDB::bind_method(D_METHOD("set_direction", "direction"), &Bullet::set_direction);
	ClassDB::bind_method(D_METHOD("get_direction"), &Bullet::get_direction);

	ClassDB::bind_method(D_METHOD("set_type", "type"), &Bullet::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &Bullet::get_type);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lifetime"), "set_lifetime", "get_lifetime");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "direction"), "set_direction", "get_direction");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), "set_type", "get_type");
}

Bullet::Bullet() {
	active = false;
	lifetime = 0.0;

	direction = Vector2(1, 0);
	perp_offset = Vector2(0, 0);

	type = Ref<BulletType>();
}

Bullet::~Bullet() {
}