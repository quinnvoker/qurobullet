#ifndef BULLETSPAWNER_H
#define BULLETSPAWNER_H

#include "scene/2d/node_2d.h"
#include "bullet_type.h"
#include <map>

class BulletSpawner : public Node2D{
    GDCLASS(BulletSpawner, Node2D);

    bool autofire;
    int interval_frames;

    Ref<BulletType> bullet_type;

    Array shots;
    bool shots_update_required;

    float spawn_radius;
    float spawn_angle;

    int bullet_count;

    float spread;
    float volley_offset;

    int scatter_type;
    float scatter_range;

    bool inherit_rotation;
    float self_rotation;

    bool inherit_scale;
    Vector2 self_scale;

    bool in_game_preview;
    Color preview_color;

    void _ready();
    void _process(float delta);
    void _physics_process(float delta);

    Array _update_shots();
    Vector2 _get_spawn_position(const Vector2 &p_shot_dir);

    void _reset_global_transform();
    void _draw_shot_preview(const Color &p_border_col, const Color &p_shot_col);
    void _draw_adjusted_arc(float p_inner_rad, float p_outer_rad, const Vector2 &p_arc_start, int p_point_count, const Color &p_color);

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    BulletSpawner();
    ~BulletSpawner();

    enum ScatterType {
        VOLLEY,
        BULLET,
    };

    void fire();

    void set_autofire(bool p_enabled);
    bool get_autofire() const;

    void set_interval_frames(int p_interval);
    int get_interval_frames() const;

    void set_bullet_type(const Ref<BulletType> &p_type);
    Ref<BulletType> get_bullet_type() const;

    Array get_shots();
    Array get_scattered_shots();

    void set_spawn_radius(float p_radius);
    float get_spawn_radius() const;

    void set_spawn_angle(float p_radians);
    float get_spawn_angle() const;

    void set_spawn_angle_degrees(float p_degrees);
    float get_spawn_angle_degrees() const;

    void set_bullet_count(int p_count);
    int get_bullet_count() const;

    void set_scatter_type(int p_type);
    int get_scatter_type() const;

    void set_scatter_range(float p_radians);
    float get_scatter_range() const;

    void set_scatter_range_degrees(float p_degrees);
    float get_scatter_range_degrees() const;

    void set_self_rotation(float p_radians);
    float get_self_rotation() const;

    void set_self_rotation_degrees(float p_degrees);
    float get_self_rotation_degrees() const;

    void set_adjusted_global_rotation(float p_radians);
    float get_adjusted_global_rotation() const;

    void set_self_scale(const Vector2 &p_scale);
    float get_self_scale() const;

    void set_adjusted_global_scale(const Vector2 &p_scale);
    float get_adjusted_global_scale() const;
};


#endif