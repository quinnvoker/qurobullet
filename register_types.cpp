/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "core/engine.h"
#include "bullet_type.h"
#include "bullet.h"
#include "bullet_server.h"
#include "bullet_server_relay.h"

void register_qurobullet_types() {
    ClassDB::register_class<BulletType>();
    ClassDB::register_class<Bullet>();
    ClassDB::register_class<BulletServer>();
    ClassDB::register_class<BulletServerRelay>();

    Engine::get_singleton()->add_singleton(Engine::Singleton("BulletServerRelay", memnew(BulletServerRelay)));
}

void unregister_qurobullet_types() 
{
}