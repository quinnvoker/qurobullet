/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "bullet_type.h"
#include "bullet.h"
#include "bullet_server.h"

void register_qurobullet_types() {
    ClassDB::register_class<BulletType>();
    ClassDB::register_class<Bullet>();
    ClassDB::register_class<BulletServer>();
}

void unregister_qurobullet_types() {
   // Nothing to do here in this example.
}