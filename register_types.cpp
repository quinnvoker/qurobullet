/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "summator.h"
#include "bullet_type.h"

void register_qurobullet_types() {
    ClassDB::register_class<Summator>();
    ClassDB::register_class<BulletType>();
}

void unregister_qurobullet_types() {
   // Nothing to do here in this example.
}