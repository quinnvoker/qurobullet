#ifndef BULLETSPAWNER_H
#define BULLETSPAWNER_H

#include "core/engine.h"
#include "scene/2d/node_2d.h"
#include "bullet_type.h"
#include "bullet_server_relay.h"

class BulletSpawner : public Node2D{
    GDCLASS(BulletSpawner, Node2D);

public:
    enum PatternMode {
        ALL,
        MANUAL,
    };

    enum AimMode {
        RADIAL,
        UNIFORM,
        TARGET_LOCAL,
        TARGET_GLOBAL,
    };

    enum ScatterMode {
        NONE,
        BULLET,
        VOLLEY,
    };

private:
    bool autofire;
    float _autofire_time;
    int interval_frames;

    Ref<BulletType> bullet_type;

    int shot_count;

    float radius;
    float arc_width;
    float arc_rotation;
    float arc_offset;

    AimMode aim_mode;
    float aim_angle;
    Vector2 aim_target_position;

    ScatterMode scatter_mode;
    float scatter_range;

    PatternMode pattern_mode;
    PoolIntArray active_shot_indices;

    bool inherit_rotation;
    float rotation_modifier;

    bool inherit_scale;
    Vector2 scale_modifier;

    bool in_game_preview;
    Color preview_color;

    Array _cached_volley;
    bool _cache_update_required;

    Transform2D _previous_transform;

    Array _get_active_shots(const Array &p_volley, const PoolIntArray &p_shot_indices);

    void _update_cached_volley();

    Array _create_volley() const;
    Vector2 _get_shot_position(const Vector2 &p_normal) const;
    Vector2 _get_shot_direction(const Vector2 &p_position, const Vector2 &p_normal) const;

    void _draw_shot_preview(const Color &p_border_col, const Color &p_shot_col);
    void _draw_adjusted_arc(float p_inner_rad, float p_outer_rad, const Vector2 &p_arc_start, int p_point_count, const Color &p_color);

protected:
    static void _bind_methods();
    void _notification(int p_what);
    void _validate_property(PropertyInfo &property) const;

public:
    void fire();
    void fire_shots(const PoolIntArray &p_shot_indices);

    Array get_volley();
    Array get_scattered_volley();

    void set_autofire(bool p_enabled);
    bool get_autofire() const;

    void set_interval_frames(int p_interval);
    int get_interval_frames() const;

    void set_bullet_type(const Ref<BulletType> &p_type);
    Ref<BulletType> get_bullet_type() const;

    void set_shot_count(int p_count);
    int get_shot_count() const;

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

    void set_aim_target_position(const Vector2 &p_position);
    Vector2 get_aim_target_position() const;

    void set_scatter_mode(ScatterMode p_mode);
    ScatterMode get_scatter_mode() const;

    void set_scatter_range(float p_radians);
    float get_scatter_range() const;

    void set_scatter_range_degrees(float p_degrees);
    float get_scatter_range_degrees() const;

    void set_pattern_mode(PatternMode p_mode);
    PatternMode get_pattern_mode() const;

    void set_active_shot_indices(const PoolIntArray &p_points);
    PoolIntArray get_active_shot_indices() const;

    void set_inherit_rotation(bool p_enabled);
    bool get_inherit_rotation() const;

    void set_rotation_modifier(float p_radians);
    float get_rotation_modifier() const;

    void set_rotation_modifier_degrees(float p_degrees);
    float get_rotation_modifier_degrees() const;

    void set_adjusted_global_rotation(float p_radians);
    float get_adjusted_global_rotation() const;

    void set_inherit_scale(bool p_enabled);
    bool get_inherit_scale() const;

    void set_scale_mod(const Vector2 &p_scale);
    Vector2 get_scale_mod() const;

    void set_adjusted_global_scale(const Vector2 &p_scale);
    Vector2 get_adjusted_global_scale() const;

    BulletSpawner();
    ~BulletSpawner();
};

VARIANT_ENUM_CAST(BulletSpawner::PatternMode)
VARIANT_ENUM_CAST(BulletSpawner::AimMode)
VARIANT_ENUM_CAST(BulletSpawner::ScatterMode)

#endif