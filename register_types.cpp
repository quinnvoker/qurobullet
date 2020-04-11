/* register_types.cpp */

#include "register_types.h"

#include "bullet.h"
#include "bullet_data.h"
#include "bullet_server.h"
#include "bullet_server_relay.h"
#include "bullet_spawner.h"
#include "core/class_db.h"
#include "core/engine.h"

void register_qurobullet_types() {
	ClassDB::register_class<BulletData>();
	ClassDB::register_class<Bullet>();
	ClassDB::register_class<BulletServer>();
	ClassDB::register_class<BulletServerRelay>();
	ClassDB::register_class<BulletSpawner>();

	Engine::get_singleton()->add_singleton(Engine::Singleton("BulletServerRelay", memnew(BulletServerRelay)));
}

void unregister_qurobullet_types() {
}