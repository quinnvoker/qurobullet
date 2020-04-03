#include "bullet_server.h"

#include "core/engine.h"
#include "bullet_server_relay.h"
#include "scene/resources/world_2d.h"
#include "servers/physics_2d_server.h"
#include <algorithm>

void BulletServer::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &BulletServer::_physics_process);
    ClassDB::bind_method(D_METHOD("spawn_bullet", "type", "position", "direction"), &BulletServer::spawn_bullet);
    ClassDB::bind_method(D_METHOD("spawn_volley", "type", "position", "shots"), &BulletServer::spawn_volley);

    ClassDB::bind_method(D_METHOD("set_bullet_pool_size", "size"), &BulletServer::set_bullet_pool_size);
    ClassDB::bind_method(D_METHOD("get_bullet_pool_size"), &BulletServer::get_bullet_pool_size);

    ClassDB::bind_method(D_METHOD("set_play_area_margin", "margin"), &BulletServer::set_play_area_margin);
    ClassDB::bind_method(D_METHOD("get_play_area_margin"), &BulletServer::get_play_area_margin);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "bullet_pool_size"), "set_bullet_pool_size", "get_bullet_pool_size");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "play_area_margin"), "set_play_area_margin", "get_play_area_margin");

    ADD_SIGNAL(MethodInfo("object_hit", PropertyInfo(Variant::OBJECT, "object", PROPERTY_HINT_RESOURCE_TYPE, "CollisionObject2D"), PropertyInfo(Variant::OBJECT, "bullet", PROPERTY_HINT_RESOURCE_TYPE, "Bullet")));
}

BulletServer::BulletServer()
{
    set_physics_process(true);
    bullet_pool_size = 1500;
    play_area_margin = 0;
}

BulletServer::~BulletServer()
{
}

void BulletServer::_notification(int p_what)
{
    switch (p_what)
    {
        
        case NOTIFICATION_READY:
        {
            _ready();
        }
        break;

        case NOTIFICATION_PHYSICS_PROCESS:
        {
            _physics_process(get_physics_process_delta_time());
        }
        
        default:
            break;
    }
}

void BulletServer::_ready()
{
    if(get_tree()->is_node_being_edited(this))
        return;
    set_physics_process(true);
    play_area = get_viewport_rect().grow(play_area_margin);
    BulletServerRelay *relay = Object::cast_to<BulletServerRelay>(Engine::get_singleton()->get_singleton_object("BulletServerRelay"));
    relay->connect("bullet_spawn_requested", this, "spawn_bullet");
    relay->connect("volley_spawn_requested", this, "spawn_volley");
    _init_bullets();
}

void BulletServer::_physics_process(float delta)
{
    if(get_tree()->is_node_being_edited(this))
        return;
    std::vector<int> bullet_indices_to_clear;
    Physics2DDirectSpaceState *space_state = get_world_2d()->get_direct_space_state();
    Physics2DDirectSpaceState::ShapeResult results;
    for( int i = 0; i < int(live_bullets.size()); i++ )
    {
        Bullet *bullet = live_bullets[i];
        if(play_area.has_point(bullet->get_position()))
        {
            bullet->move(delta);
            int collisions = bullet->intersect_shape(*space_state, &results);
            if(collisions > 0)
            {
                emit_signal("object_hit", results.collider, bullet);
                bullet_indices_to_clear.push_back(i);
            }
            else
            {
                bullet->set_lifetime(bullet->get_lifetime() + delta);
            }           
        }
        else
        {
            bullet_indices_to_clear.push_back(i);
        }
    }
    for(int i = 0; i < int(bullet_indices_to_clear.size()); i++)
    {
        Bullet *bullet = live_bullets[bullet_indices_to_clear[i] - i];
        bullet->set_active(false);
        live_bullets.erase(live_bullets.begin() + bullet_indices_to_clear[i] - i);
        dead_bullets.insert(dead_bullets.begin(), bullet);
    }
}

void BulletServer::spawn_bullet(Ref<BulletType> type, Vector2 position, Vector2 direction)
{
    Bullet *bullet;

    if(dead_bullets.size() > 0)
    {
        bullet = dead_bullets.back();
        dead_bullets.pop_back();
        bullet->set_active(true);
    }
    else
    {
        bullet = live_bullets.back();
        live_bullets.pop_back();
    }

    bullet->set_position(position);
    bullet->set_direction(direction);
    bullet->set_type(type);


    live_bullets.insert(live_bullets.begin(), bullet);
}

void BulletServer::spawn_volley(Ref<BulletType> type, Vector2 position, Array shots)
{
    for(int i = 0; i < shots.size(); i++)
    {
        Dictionary shot = shots[i];
        spawn_bullet(type, position + shot["offset"], shot["direction"]);
        shot.empty();
    }
}

void BulletServer::kill_bullet(Bullet* bullet)
{
    bullet->set_active(false);
    dead_bullets.insert(dead_bullets.begin(), bullet);
    auto it = std::find(live_bullets.begin(), live_bullets.end(), bullet);
    live_bullets.erase(it);
}

void BulletServer::_init_bullets()
{
    for(int i = 0; i < bullet_pool_size; i++)
    {
        _create_bullet();
    }
}


void BulletServer::_create_bullet()
{
    Bullet* bullet = memnew(Bullet);
    add_child(bullet);
    dead_bullets.insert(dead_bullets.begin(), bullet);
}

void BulletServer::set_bullet_pool_size(int value)
{
    if(value > -1)
        bullet_pool_size = value;
    if(!is_inside_tree() || get_tree()->is_node_being_edited(this))
        return;
    while(int(dead_bullets.size() + live_bullets.size()) < bullet_pool_size)
        _create_bullet();
    while(int(dead_bullets.size() + live_bullets.size()) > bullet_pool_size)
    {
        Bullet *bullet;
        if(dead_bullets.size() > 0)
        {
            bullet = dead_bullets.back();
            dead_bullets.pop_back();
        }
        else
        {
            bullet = live_bullets.back();
            live_bullets.pop_back();
        }
        remove_child(bullet);
        memdelete(bullet);
    }
}

int BulletServer::get_bullet_pool_size()
{
    return bullet_pool_size;
}

void BulletServer::set_play_area_margin(int value)
{
    play_area_margin = value;
    if(!is_inside_tree() || get_tree()->is_node_being_edited(this))
        return;
    play_area = get_viewport_rect().grow(play_area_margin);
}

int BulletServer::get_play_area_margin()
{
    return play_area_margin;
}