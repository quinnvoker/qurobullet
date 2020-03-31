#ifndef BULLETTYPE_H
#define BULLETTYPE_H

#include "core/reference.h"
#include "core/resource.h"
#include "scene/resources/texture.h"
#include "scene/resources/material.h"
#include "scene/resources/shape_2d.h"

class BulletType : public Resource{
    GDCLASS(BulletType, Resource);

    private:
        Ref<Texture> texture;
        Ref<Material> material;
        float damage;
        Ref<Shape2D> collision_shape;
        int collision_mask;
        float scale;
        float speed;
        float linear_acceleration;
        float curve_rate;
        float sin_amplitude;
        float sin_frequency;
    
    protected:
        static void _bind_methods();

    public:
        BulletType();
        ~BulletType();

        void set_texture(Ref<Texture> value);
        Ref<Texture> get_texture();

        void set_material(Ref<Material> value);
        Ref<Material> get_material();

        void set_damage(float value);
        float get_damage();

        void set_collision_shape(Ref<Shape2D> value);
        Ref<Shape2D> get_collision_shape();

        void set_collision_mask(int value);
        int get_collision_mask();

        void set_scale(float value);
        float get_scale();

        void set_speed(float value);
        float get_speed();

        void set_linear_acceleration(float value);
        float get_linear_acceleration();

        void set_curve_rate(float value);
        float get_curve_rate();

        void set_sin_amplitude(float value);
        float get_sin_amplitude();

        void set_sin_frequency(float value);
        float get_sin_frequency();
};


#endif