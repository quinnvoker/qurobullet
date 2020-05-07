#ifndef BULLETTYPE_H
#define BULLETTYPE_H

#include "core/resource.h"
#include "scene/resources/material.h"
#include "scene/resources/shape_2d.h"
#include "scene/resources/texture.h"

class BulletType : public Resource {
	GDCLASS(BulletType, Resource);

public:
	enum WaveType {
		NONE,
		SIN,
		COS,
	};

private:
	float speed;
	float damage;
	float lifetime;
	Ref<Texture> texture;
	Color modulate;
	int light_mask;
	Ref<Material> material;
	Ref<Shape2D> collision_shape;
	int collision_mask;
	bool collision_detect_bodies;
	bool collision_detect_areas;
	float linear_acceleration;
	float curve_rate;
	WaveType h_wave_type;
	float h_wave_amplitude;
	float h_wave_frequency;
	WaveType v_wave_type;
	float v_wave_amplitude;
	float v_wave_frequency;
	bool face_direction;
	float rotation;
	Vector2 scale;
	Dictionary custom_data;

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;

public:
	void set_speed(float p_speed);
	float get_speed() const;

	void set_damage(float p_amount);
	float get_damage() const;

	void set_lifetime(float p_time);
	float get_lifetime() const;

	void set_texture(const Ref<Texture> &p_texture);
	Ref<Texture> get_texture() const;

	void set_modulate(const Color &p_color);
	Color get_modulate() const;

	void set_light_mask(int p_mask);
	int get_light_mask() const;

	void set_material(const Ref<Material> &p_material);
	Ref<Material> get_material() const;

	void set_collision_shape(const Ref<Shape2D> &p_shape);
	Ref<Shape2D> get_collision_shape() const;

	void set_collision_mask(int p_mask);
	int get_collision_mask() const;

	void set_collision_detect_bodies(bool p_enabled);
	bool get_collision_detect_bodies() const;

	void set_collision_detect_areas(bool p_enabled);
	bool get_collision_detect_areas() const;

	void set_linear_acceleration(float p_acceleration);
	float get_linear_acceleration() const;

	void set_curve_rate(float p_degrees_per_sec);
	float get_curve_rate() const;

	void set_h_wave_type(WaveType p_type);
	WaveType get_h_wave_type() const;

	void set_h_wave_amplitude(float p_amplitude);
	float get_h_wave_amplitude() const;

	void set_h_wave_frequency(float p_freq);
	float get_h_wave_frequency() const;

	void set_v_wave_type(WaveType p_type);
	WaveType get_v_wave_type() const;

	void set_v_wave_amplitude(float p_amplitude);
	float get_v_wave_amplitude() const;

	void set_v_wave_frequency(float p_freq);
	float get_v_wave_frequency() const;

	void set_face_direction(bool p_enabled);
	bool get_face_direction() const;

	void set_rotation(float p_radians);
	float get_rotation() const;

	void set_rotation_degrees(float p_degrees);
	float get_rotation_degrees() const;

	void set_scale(Vector2 p_scale);
	Vector2 get_scale() const;

	void set_custom_data(const Dictionary &p_data);
	Dictionary get_custom_data() const;

	BulletType();
	~BulletType();
};

VARIANT_ENUM_CAST(BulletType::WaveType)

#endif