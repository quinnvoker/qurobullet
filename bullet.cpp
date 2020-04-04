#include "bullet.h"

#include "core/os/os.h"

void Bullet::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("move", "delta"), &Bullet::move);

    ClassDB::bind_method(D_METHOD("set_active", "active"), &Bullet::set_active);
    ClassDB::bind_method(D_METHOD("get_active"), &Bullet::get_active);

    ClassDB::bind_method(D_METHOD("set_direction", "direction"), &Bullet::set_direction);
    ClassDB::bind_method(D_METHOD("get_direction"), &Bullet::get_direction);

    ClassDB::bind_method(D_METHOD("set_type", "type"), &Bullet::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &Bullet::get_type);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "direction"), "set_direction", "get_direction");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "BulletType"), "set_type", "get_type");
}

Bullet::Bullet()
{
    active = false;
    lifetime = 0.0;

    direction = Vector2(1,0);
    perp_offset = Vector2(0,0);

    type = Ref<BulletType>();
}

Bullet::~Bullet()
{
}

void Bullet::_ready()
{
    if(get_tree()->is_node_being_edited(this))
        return;
    set_active(false);
}

void Bullet::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_DRAW:
        {
            if(type.is_null() or type->get_texture().is_null())
                return;
            Ref<Texture> tex = type->get_texture();
            RID ci = get_canvas_item();

            tex->draw(ci, -tex->get_size() / 2);
        }
            
            break;
        
        case NOTIFICATION_READY:
        {
            _ready();
        }
        
        default:
            break;
    }
}

void Bullet::move(float delta)
{
    float current_speed = type->get_speed() + type->get_linear_acceleration() * lifetime;
    direction = direction.rotated(type->get_curve_rate() * delta * Math_PI / 180);
    Vector2 perpendicular = direction.rotated(90 * Math_PI / 180);
    float sin_point = sin(lifetime * type->get_sin_frequency());
    Vector2 new_perp_offset = perpendicular * sin_point * type->get_sin_amplitude();
    set_position(get_position() - perp_offset + direction * current_speed * delta + new_perp_offset);
    perp_offset = new_perp_offset;
}

int Bullet::intersect_shape(Physics2DDirectSpaceState &p_space_state, Physics2DDirectSpaceState::ShapeResult *r_results)
{
    int collisions = p_space_state.intersect_shape(type->get_collision_shape()->get_rid(), get_transform(), Vector2(0,0), 0.0, r_results, 1, Set<RID>(), type->get_collision_mask(), true, true);
    return collisions;
}

void Bullet::set_active(bool value)
{
    active = value;
    set_visible(value);
}

bool Bullet::get_active()
{
    return active;
}

void Bullet::set_lifetime(float value)
{
    lifetime = value;
}

float Bullet::get_lifetime()
{
    return lifetime;
}

void Bullet::set_direction(Vector2 value)
{
    direction = value;
    if(get_tree()->is_node_being_edited(this))
        return;
    if(!type.is_null() && type->get_face_direction())
        set_rotation(direction.angle());
    else
        set_rotation(0.0);  
}

Vector2 Bullet::get_direction()
{
    return direction;
}

void Bullet::set_type(Ref<BulletType> value)
{
    lifetime = 0.0;
    perp_offset = Vector2(0,0);
    type = value;
    set_scale(Vector2(type->get_scale(), type->get_scale()));
    set_material(type->get_material());
    update();
}

Ref<BulletType> Bullet::get_type()
{
    return type;
}