/* register_types.cpp */

#include "register_types.h"

#include "bullet.h"
#include "bullet_server.h"
#include "bullet_server_relay.h"
#include "bullet_spawner.h"
#include "bullet_type.h"
#include "core/config/engine.h"
#include "core/object/class_db.h"

BulletServerRelay *_bullet_server_relay = nullptr;

void initialize_qurobullet_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	_bullet_server_relay = memnew(BulletServerRelay);

	GDREGISTER_CLASS(Bullet);
	GDREGISTER_CLASS(BulletType);
	GDREGISTER_CLASS(BulletServer);
	GDREGISTER_CLASS(BulletServerRelay);
	GDREGISTER_CLASS(BulletSpawner);

	Engine *engine = Engine::get_singleton();
	if (engine->has_singleton("BulletServerRelay")) {
		return;
	}

	engine->add_singleton(Engine::Singleton("BulletServerRelay", _bullet_server_relay));
}

void uninitialize_qurobullet_module(ModuleInitializationLevel p_level) {
	if (_bullet_server_relay) {
		memdelete(_bullet_server_relay);
	}
}
