#include "bullet_server_relay.h"

void BulletServerRelay::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_bullet_fired", "type", "position", "direction"), &BulletServerRelay::on_bullet_fired);
    ClassDB::bind_method(D_METHOD("on_volley_fired", "type", "position", "shots"), &BulletServerRelay::on_volley_fired);

    ADD_SIGNAL(MethodInfo("bullet_spawn_requested", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::VECTOR2, "direction")));
    ADD_SIGNAL(MethodInfo("volley_spawn_requested", PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), PropertyInfo(Variant::VECTOR2, "position"), PropertyInfo(Variant::ARRAY, "shots")));
}

BulletServerRelay::BulletServerRelay()
{
}

BulletServerRelay::~BulletServerRelay()
{
}

void BulletServerRelay::on_bullet_fired(Ref<BulletType> type, Vector2 position, Vector2 direction)
{
    emit_signal("bullet_spawn_requested", type, position, direction);
}

void BulletServerRelay::on_volley_fired(Ref<BulletType> type, Vector2 position, Array shots)
{
    emit_signal("volley_spawn_requested", type, position, shots);
}