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

void initialize_qurobullet_module(ModuleInitializationLevel p_level) {
	_bullet_server_relay = memnew(BulletServerRelay);

	GDREGISTER_CLASS(Bullet);
	GDREGISTER_CLASS(BulletType);
	GDREGISTER_CLASS(BulletServer);
	GDREGISTER_CLASS(BulletServerRelay);
	GDREGISTER_CLASS(BulletSpawner);

	Engine::get_singleton()->add_singleton(Engine::Singleton("BulletServerRelay", _bullet_server_relay));
}

void initialize_qurobullet_module(ModuleInitializationLevel p_level) {
	if (_bullet_server_relay) {
		memdelete(_bullet_server_relay);
	}
}
