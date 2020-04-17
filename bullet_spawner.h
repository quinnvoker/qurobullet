#ifndef BULLETSPAWNER_H
#define BULLETSPAWNER_H

#include "core/engine.h"
#include "scene/2d/node_2d.h"
#include "bullet_data.h"
#include "bullet_server_relay.h"

class BulletSpawner : public Node2D{
    GDCLASS(BulletSpawner, Node2D);

public:
    enum AimMode {
        RADIAL,
        UNIFORM,
        TARGET_LOCAL,
        TARGET_GLOBAL,
    };

    enum ScatterType {
        NONE,
        BULLET,
        VOLLEY,
    };

private:
    bool autofire;
    float _autofire_time;
    int interval_frames;

    int bullet_count;
    Ref<BulletData> bullet_type;

    float radius;
    float arc_width;
    float arc_rotation;
    float arc_offset;

    AimMode aim_mode;
    float aim_angle;
    Vector2 target_position;

    ScatterType scatter_type;
    float scatter_range;

    bool inherit_rotation;
    float self_rotation;

    bool inherit_scale;
    Vector2 self_scale;

    bool in_game_preview;
    Color preview_color;

    Array _cached_shots;
    bool _cache_update_required;

    Transform2D _previous_transform;

    void _ready();
    void _process_internal(float delta);
    void _physics_process_internal(float delta);

    void _update_cached_shots();

    Array _create_volley() const;
    Vector2 _get_shot_direction(const Vector2 &p_position, const Vector2 &p_normal) const;

    void _draw_shot_preview(const Color &p_border_col, const Color &p_shot_col);
    void _draw_adjusted_arc(float p_inner_rad, float p_outer_rad, const Vector2 &p_arc_start, int p_point_count, const Color &p_color);

protected:
    static void _bind_methods();
    void _notification(int p_what);
    void _validate_property(PropertyInfo &property) const;

public:
    void fire();

    Array get_shots();
    Array get_scattered_shots();

    void set_autofire(bool p_enabled);
    bool get_autofire() const;

    void set_interval_frames(int p_interval);
    int get_interval_frames() const;

    void set_bullet_count(int p_count);
    int get_bullet_count() const;

    void set_bullet_data(const Ref<BulletData> &p_type);
    Ref<BulletData> get_bullet_data() const;

    void set_radius(float p_radius);
    float get_radius() const;

    void set_arc_width(float p_radians);
    float get_arc_width() const;

    void set_arc_width_degrees(float p_degrees);
    float get_arc_width_degrees() const;

    void set_arc_rotation(float p_radians);
    float get_arc_rotation() const;

    void set_arc_rotation_degrees(float p_degrees);
    float get_arc_rotation_degrees() const;

    void set_arc_offset(float p_offset);
    float get_arc_offset() const;

    void set_aim_mode(AimMode p_mode);
    AimMode get_aim_mode() const;

    void set_aim_angle(float p_radians);
    float get_aim_angle() const;

    void set_aim_angle_degrees(float p_degrees);
    float get_aim_angle_degrees() const;

    void set_target_position(const Vector2 &p_position);
    Vector2 get_target_position() const;

    void set_scatter_type(ScatterType p_type);
    ScatterType get_scatter_type() const;

    void set_scatter_range(float p_radians);
    float get_scatter_range() const;

    void set_scatter_range_degrees(float p_degrees);
    float get_scatter_range_degrees() const;

    void set_inherit_rotation(bool p_enabled);
    bool get_inherit_rotation() const;

    void set_self_rotation(float p_radians);
    float get_self_rotation() const;

    void set_self_rotation_degrees(float p_degrees);
    float get_self_rotation_degrees() const;

    void set_adjusted_global_rotation(float p_radians);
    float get_adjusted_global_rotation() const;

    void set_inherit_scale(bool p_enabled);
    bool get_inherit_scale() const;

    void set_self_scale(const Vector2 &p_scale);
    Vector2 get_self_scale() const;

    void set_adjusted_global_scale(const Vector2 &p_scale);
    Vector2 get_adjusted_global_scale() const;

    BulletSpawner();
    ~BulletSpawner();
};

VARIANT_ENUM_CAST(BulletSpawner::AimMode)
VARIANT_ENUM_CAST(BulletSpawner::ScatterType)

#endif