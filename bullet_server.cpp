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

		case NOTIFICATION_INTERNAL_PROCESS: {
			_process_internal(get_process_delta_time());
		}
		break;

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
	set_process_internal(true);
	set_physics_process_internal(true);
	play_area = get_viewport_rect().grow(play_area_margin);
	BulletServerRelay *relay = Object::cast_to<BulletServerRelay>(Engine::get_singleton()->get_singleton_object("BulletServerRelay"));
	relay->connect("bullet_spawn_requested", this, "spawn_bullet");
	relay->connect("volley_spawn_requested", this, "spawn_volley");
	_init_bullets();
}

void BulletServer::_process_internal(float delta) {
	for (int i = 0; i < live_bullets.size(); i++){
		Bullet *b = live_bullets[i];
		VS::get_singleton()->canvas_item_set_transform(b->get_ci_rid(), b->get_transform());
	}
}

void BulletServer::_physics_process_internal(float delta) {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	Vector<int> bullet_indices_to_clear;
	Physics2DDirectSpaceState *space_state = get_world_2d()->get_direct_space_state();
	Vector<Physics2DDirectSpaceState::ShapeResult> results;
	results.resize(32);

	for (int i = 0; i < live_bullets.size(); i++) {
		Bullet *bullet = live_bullets[i];
		VS::get_singleton()->canvas_item_set_draw_index(bullet->get_ci_rid(), i);

		if (bullet->is_popped()){
			bullet_indices_to_clear.push_back(i);
		} else if (max_lifetime >= 0.001 && bullet->get_time() > max_lifetime){
			bullet->pop();
		} else if (play_area.has_point(bullet->get_position())) {
			bullet->update(delta);
			if (!bullet->can_collide()){
				continue;
			}
			Ref<BulletData> b_data = bullet->get_data();
			int collisions = space_state->intersect_shape(b_data->get_collision_shape()->get_rid(), bullet->get_transform(), Vector2(0,0), 0, results.ptrw(), results.size(), Set<RID>(), b_data->get_collision_mask(), true, true);
			if (collisions > 0){
				Array colliders;
				colliders.resize(collisions);
				for (int c = 0; c < collisions; c++){
					colliders[c] = results[c].collider;
				}
				emit_signal("collision_detected", bullet, colliders);
				if(pop_on_collide){
					bullet->pop();
				}
			}
		} else {
			bullet->pop();
		}
		
	}

	for (int i = 0; i < bullet_indices_to_clear.size(); i++) {
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
	VS::get_singleton()->canvas_item_set_parent(bullet->get_ci_rid(), get_canvas_item());
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
	VS::get_singleton()->canvas_item_set_draw_index(bullet->get_ci_rid(), 0);
	live_bullets.insert(0, bullet);
}

void BulletServer::spawn_volley(const Ref<BulletData> &p_type, const Vector2 &p_position, const Array &p_volley) {
	for (int i = 0; i < p_volley.size(); i++) {
		Dictionary shot = p_volley[i];
		spawn_bullet(p_type, p_position + shot["position"], shot["direction"]);
		shot.empty();
	}
}

void BulletServer::clear_bullets(){
	for (int i = 0; i < live_bullets.size(); i++){
		live_bullets[i]->pop();
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


void BulletServer::set_max_lifetime(float p_time) {
	max_lifetime = p_time;
}

float BulletServer::get_max_lifetime() const {
	return max_lifetime;
}

void BulletServer::set_pop_on_collide(bool p_enabled){
	pop_on_collide = p_enabled;
}

bool BulletServer::get_pop_on_collide() const {
	return pop_on_collide;
}

void BulletServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("spawn_bullet", "type", "position", "direction"), &BulletServer::spawn_bullet);
	ClassDB::bind_method(D_METHOD("spawn_volley", "type", "position", "volley"), &BulletServer::spawn_volley);
	ClassDB::bind_method(D_METHOD("clear_bullets"), &BulletServer::clear_bullets);


	ClassDB::bind_method(D_METHOD("set_bullet_pool_size", "size"), &BulletServer::set_bullet_pool_size);
	ClassDB::bind_method(D_METHOD("get_bullet_pool_size"), &BulletServer::get_bullet_pool_size);

	ClassDB::bind_method(D_METHOD("set_play_area_margin", "margin"), &BulletServer::set_play_area_margin);
	ClassDB::bind_method(D_METHOD("get_play_area_margin"), &BulletServer::get_play_area_margin);

	ClassDB::bind_method(D_METHOD("set_pop_on_collide", "enabled"), &BulletServer::set_pop_on_collide);
	ClassDB::bind_method(D_METHOD("get_pop_on_collide"), &BulletServer::get_pop_on_collide);

	ClassDB::bind_method(D_METHOD("set_max_lifetime", "time"), &BulletServer::set_max_lifetime);
	ClassDB::bind_method(D_METHOD("get_max_lifetime"), &BulletServer::get_max_lifetime);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "bullet_pool_size", PROPERTY_HINT_RANGE, "1,5000,1,or_greater"), "set_bullet_pool_size", "get_bullet_pool_size");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "play_area_margin", PROPERTY_HINT_RANGE, "0,300,0.01,or_lesser,or_greater"), "set_play_area_margin", "get_play_area_margin");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_lifetime", PROPERTY_HINT_RANGE, "0,300,0.01,or_greater"), "set_max_lifetime", "get_max_lifetime");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "pop_on_collide"), "set_pop_on_collide", "get_pop_on_collide");

	ADD_SIGNAL(MethodInfo("collision_detected", PropertyInfo(Variant::OBJECT, "bullet", PROPERTY_HINT_RESOURCE_TYPE, "Bullet"), PropertyInfo(Variant::ARRAY, "colliders")));
}

BulletServer::BulletServer() {
	set_physics_process(true);
	bullet_pool_size = 1500;
	play_area_margin = 0;
	pop_on_collide = true;
}

BulletServer::~BulletServer() {
}