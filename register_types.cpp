/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "bullet_type.h"
#include "bullet.h"

void register_qurobullet_types() {
    ClassDB::register_class<BulletType>();
    ClassDB::register_class<Bullet>();
}

void unregister_qurobullet_types() {
   // Nothing to do here in this example.
}