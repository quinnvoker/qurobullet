/* register_types.cpp */

#include "register_types.h"

#include "bullet.h"
#include "bullet_server.h"
#include "bullet_server_relay.h"
#include "bullet_spawner.h"
#include "bullet_type.h"
#include "core/class_db.h"
#include "core/engine.h"

BulletServerRelay *_bullet_server_relay = nullptr;

void register_qurobullet_types() {
	_bullet_server_relay = memnew(BulletServerRelay);

	ClassDB::register_class<Bullet>();
	ClassDB::register_class<BulletType>();
	ClassDB::register_class<BulletServer>();
	ClassDB::register_class<BulletServerRelay>();
	ClassDB::register_class<BulletSpawner>();

	Engine::get_singleton()->add_singleton(Engine::Singleton("BulletServerRelay", _bullet_server_relay));
}

void unregister_qurobullet_types() {
	if (_bullet_server_relay) {
		memdelete(_bullet_server_relay);
	}
}
