#include "bullet_server.h"

#include "bullet_server_relay.h"
#include "core/engine.h"
#include "scene/resources/world_2d.h"
#include "servers/physics_2d_server.h"

void BulletServer::_notification(int p_what) {
	switch (p_what) {

		case NOTIFICATION_READY: {
			_ready();
		} break;

		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			_physics_process_internal(get_physics_process_delta_time());
		}
		break;

		default:
			break;
	}
}

void BulletServer::_ready() {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	set_physics_process_internal(true);
	play_area = get_viewport_rect().grow(play_area_margin);
	BulletServerRelay *relay = Object::cast_to<BulletServerRelay>(Engine::get_singleton()->get_singleton_object("BulletServerRelay"));
	relay->connect("bullet_spawn_requested", this, "spawn_bullet");
	relay->connect("volley_spawn_requested", this, "spawn_volley");
	_init_bullets();
}

void BulletServer::_physics_process_internal(float delta) {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	Vector<int> bullet_indices_to_clear;
	Physics2DDirectSpaceState *space_state = get_world_2d()->get_direct_space_state();
	Vector<Physics2DDirectSpaceState::ShapeResult> results;
	results.resize(32);

	for (int i = 0; i < int(live_bullets.size()); i++) {
		Bullet *bullet = live_bullets[i];

		if (bullet->is_popped() || !play_area.has_point(bullet->get_position())){
			bullet_indices_to_clear.push_back(i);
		} else {
			bullet->update_position(delta);
			Ref<BulletData> b_data = bullet->get_data();
			int collisions = space_state->intersect_shape(b_data->get_collision_shape()->get_rid(), bullet->get_transform(), Vector2(0,0), 0, results.ptrw(), results.size(), Set<RID>(), b_data->get_collision_mask(), true, true);
			if (collisions > 0){
				Array colliders;
				colliders.resize(collisions);
				for (int c = 0; c < collisions; c++){
					colliders[c] = results[c].collider;
				}
				emit_signal("collision_detected", bullet, colliders);
				if(auto_pop){
					bullet->pop();
				}
			}
		}
	}

	for (int i = 0; i < int(bullet_indices_to_clear.size()); i++) {
		Bullet *bullet = live_bullets[bullet_indices_to_clear[i] - i];
		live_bullets.remove(bullet_indices_to_clear[i] - i);
		dead_bullets.insert(0, bullet);
	}
}

void BulletServer::_init_bullets() {
	for (int i = 0; i < bullet_pool_size; i++) {
		_create_bullet();
	}
}

void BulletServer::_create_bullet() {
	Bullet *bullet = memnew(Bullet);
	bullet->set_ci_rid(VS::get_singleton()->canvas_item_create());
	dead_bullets.insert(0, bullet);
}

void BulletServer::spawn_bullet(const Ref<BulletData> &p_type, const Vector2 &p_position, const Vector2 &p_direction) {
	Bullet *bullet;

	if (dead_bullets.size() > 0) {
		bullet = dead_bullets.get(dead_bullets.size() - 1);
		dead_bullets.remove(dead_bullets.size() - 1);
	} else {
		bullet = live_bullets.get(live_bullets.size() - 1);
		live_bullets.remove(live_bullets.size() - 1);
	}

	bullet->spawn(p_type, p_position, p_direction);
	live_bullets.insert(0, bullet);
}

void BulletServer::spawn_volley(const Ref<BulletData> &p_type, const Vector2 &p_position, const Array &p_shots) {
	for (int i = 0; i < p_shots.size(); i++) {
		Dictionary shot = p_shots[i];
		spawn_bullet(p_type, p_position + shot["offset"], shot["direction"]);
		shot.empty();
	}
}

void BulletServer::set_bullet_pool_size(int p_size) {
	if (p_size > -1)
		bullet_pool_size = p_size;
	if (!is_inside_tree() || Engine::get_singleton()->is_editor_hint())
		return;
	while (int(dead_bullets.size() + live_bullets.size()) < bullet_pool_size)
		_create_bullet();
	while (int(dead_bullets.size() + live_bullets.size()) > bullet_pool_size) {
		Bullet *bullet;
		if (dead_bullets.size() > 0) {
			bullet = dead_bullets.get(dead_bullets.size() - 1);
		dead_bullets.remove(dead_bullets.size() - 1);
		} else {
			bullet = live_bullets.get(live_bullets.size() - 1);
			live_bullets.remove(live_bullets.size() - 1);
		}
		VS::get_singleton()->free(bullet->get_ci_rid());
		memdelete(bullet);
	}
}

int BulletServer::get_bullet_pool_size() const {
	return bullet_pool_size;
}

void BulletServer::set_play_area_margin(float p_margin) {
	play_area_margin = p_margin;
	if (!is_inside_tree() || Engine::get_singleton()->is_editor_hint())
		return;
	play_area = get_viewport_rect().grow(play_area_margin);
}

float BulletServer::get_play_area_margin() const {
	return play_area_margin;
}

void BulletServer::set_auto_pop(bool p_enabled){
	auto_pop = p_enabled;
}

bool BulletServer::get_auto_pop() const {
	return auto_pop;
}

void BulletServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("spawn_bullet", "type", "position", "direction"), &BulletServer::spawn_bullet);
	ClassDB::bind_method(D_METHOD("spawn_volley", "type", "position", "shots"), &BulletServer::spawn_volley);

	ClassDB::bind_method(D_METHOD("set_bullet_pool_size", "size"), &BulletServer::set_bullet_pool_size);
	ClassDB::bind_method(D_METHOD("get_bullet_pool_size"), &BulletServer::get_bullet_pool_size);

	ClassDB::bind_method(D_METHOD("set_play_area_margin", "margin"), &BulletServer::set_play_area_margin);
	ClassDB::bind_method(D_METHOD("get_play_area_margin"), &BulletServer::get_play_area_margin);

	ClassDB::bind_method(D_METHOD("set_auto_pop", "enabled"), &BulletServer::set_auto_pop);
	ClassDB::bind_method(D_METHOD("get_auto_pop"), &BulletServer::get_auto_pop);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "bullet_pool_size", PROPERTY_HINT_RANGE, "1,5000,1,or_greater"), "set_bullet_pool_size", "get_bullet_pool_size");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "play_area_margin", PROPERTY_HINT_RANGE, "0,300,0.01,or_lesser,or_greater"), "set_play_area_margin", "get_play_area_margin");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_pop"), "set_auto_pop", "get_auto_pop");

	ADD_SIGNAL(MethodInfo("collision_detected", PropertyInfo(Variant::OBJECT, "bullet", PROPERTY_HINT_RESOURCE_TYPE, "Bullet"), PropertyInfo(Variant::ARRAY, "Colliders")));
}

BulletServer::BulletServer() {
	set_physics_process(true);
	bullet_pool_size = 1500;
	play_area_margin = 0;
	auto_pop = true;
}

BulletServer::~BulletServer() {
}