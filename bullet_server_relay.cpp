#include "bullet_server_relay.h"

void BulletServerRelay::spawn_bullet(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction) {
	emit_signal("bullet_spawn_requested", p_type, p_position, p_direction);
}

void BulletServerRelay::spawn_volley(const Ref<BulletType> &p_type, const Vector2 &p_origin, const Array &p_shots){
	emit_signal("volley_spawn_requested", p_type, p_origin, p_shots);
}

void BulletServerRelay::_bind_methods() {
	ClassDB::bind_method(D_METHOD("spawn_bullet", "type", "position", "direction"), &BulletServerRelay::spawn_bullet);
	ClassDB::bind_method(D_METHOD("spawn_volley", "type", "origin", "shots"), &BulletServerRelay::spawn_volley);

	ADD_SIGNAL(MethodInfo("bullet_spawn_requested", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::VECTOR2, "direction")));
	ADD_SIGNAL(MethodInfo("volley_spawn_requested", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::ARRAY, "shots")));
}

BulletServerRelay::BulletServerRelay() {
}

BulletServerRelay::~BulletServerRelay() {
}