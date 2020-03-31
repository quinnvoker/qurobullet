#include "bullet_type.h"

void BulletType::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_texture", "texture"), &BulletType::set_texture);
    ClassDB::bind_method(D_METHOD("get_texture"), &BulletType::get_texture);

    ClassDB::bind_method(D_METHOD("set_material", "material"), &BulletType::set_material);
    ClassDB::bind_method(D_METHOD("get_material"), &BulletType::get_material);

    ClassDB::bind_method(D_METHOD("set_damage", "damage"), &BulletType::set_damage);
    ClassDB::bind_method(D_METHOD("get_damage"), &BulletType::get_damage);

    ClassDB::bind_method(D_METHOD("set_collision_shape", "collision_shape"), &BulletType::set_collision_shape);
    ClassDB::bind_method(D_METHOD("get_collision_shape"), &BulletType::get_collision_shape);

    ClassDB::bind_method(D_METHOD("set_collision_mask", "collision_mask"), &BulletType::set_collision_mask);
    ClassDB::bind_method(D_METHOD("get_collision_mask"), &BulletType::get_collision_mask);

    ClassDB::bind_method(D_METHOD("set_scale", "scale"), &BulletType::set_scale);
    ClassDB::bind_method(D_METHOD("get_scale"), &BulletType::get_scale);

    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletType::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &BulletType::get_speed);

    ClassDB::bind_method(D_METHOD("set_linear_acceleration", "linear_acceleration"), &BulletType::set_linear_acceleration);
    ClassDB::bind_method(D_METHOD("get_linear_acceleration"), &BulletType::get_linear_acceleration);

    ClassDB::bind_method(D_METHOD("set_curve_rate", "curve_rate"), &BulletType::set_curve_rate);
    ClassDB::bind_method(D_METHOD("get_curve_rate"), &BulletType::get_curve_rate);

    ClassDB::bind_method(D_METHOD("set_sin_amplitude", "sin_amplitude"), &BulletType::set_sin_amplitude);
    ClassDB::bind_method(D_METHOD("get_sin_amplitude"), &BulletType::get_sin_amplitude);

    ClassDB::bind_method(D_METHOD("set_sin_frequency", "sin_frequency"), &BulletType::set_sin_frequency);
    ClassDB::bind_method(D_METHOD("get_sin_frequency"), &BulletType::get_sin_frequency);


    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_material", "get_material");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "damage"), "set_damage", "get_damage");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collision_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_collision_shape", "get_collision_shape");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "linear_acceleration"), "set_linear_acceleration", "get_linear_acceleration");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "curve_rate"), "set_curve_rate", "get_curve_rate");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "sin_amplitude"), "set_sin_amplitude", "get_sin_amplitude");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "sin_frequency"), "set_sin_frequency", "get_sin_frequency");
}

BulletType::BulletType()
{
    texture = Ref<Texture>();
    material = Ref<Material>();
    damage = 0.0;
    collision_shape = Ref<Shape2D>();
    collision_mask = 0;
    scale = 1.0;
    speed = 100.0;
    linear_acceleration = 0.0;
    curve_rate = 0.0;
    sin_amplitude = 0.0;
    sin_frequency = 0.0;
}

BulletType::~BulletType()
{
    // cleanup goes here
}

void BulletType::set_texture(Ref<Texture> value)
{
    texture = value;
}

Ref<Texture> BulletType::get_texture()
{
    return texture;
}

void BulletType::set_material(Ref<Material> value)
{
    material = value;
}

Ref<Material> BulletType::get_material()
{
    return material;
}

void BulletType::set_damage(float value)
{
    damage = value;
}

float BulletType::get_damage()
{
    return damage;
}

void BulletType::set_collision_shape(Ref<Shape2D> value)
{
    collision_shape = value;
}

Ref<Shape2D> BulletType::get_collision_shape()
{
    return collision_shape;
}

void BulletType::set_collision_mask(int value)
{
    collision_mask = value;
}

int BulletType::get_collision_mask()
{
    return collision_mask;
}

void BulletType::set_scale(float value)
{
    scale = value;
}

float BulletType::get_scale()
{
    return scale;
}

void BulletType::set_speed(float value)
{
    speed = value;
}

float BulletType::get_speed()
{
    return speed;
}

void BulletType::set_linear_acceleration(float value)
{
    linear_acceleration = value;
}

float BulletType::get_linear_acceleration()
{
    return linear_acceleration;
}

void BulletType::set_curve_rate(float value)
{
    curve_rate = value;
}

float BulletType::get_curve_rate()
{
    return curve_rate;
}

void BulletType::set_sin_amplitude(float value)
{
    sin_amplitude = value;
}

float BulletType::get_sin_amplitude()
{
    return sin_amplitude;
}

void BulletType::set_sin_frequency(float value)
{
    sin_frequency = value;
}

float BulletType::get_sin_frequency()
{
    return sin_frequency;
}