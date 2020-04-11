#ifndef BULLETDATA_H
#define BULLETDATA_H

#include "core/reference.h"
#include "core/resource.h"
#include "scene/resources/material.h"
#include "scene/resources/shape_2d.h"
#include "scene/resources/texture.h"

class BulletData : public Reference {
	GDCLASS(BulletData, Reference);

private:
	Ref<Texture> texture;
	Ref<Material> material;
	float damage;
	Ref<Shape2D> collision_shape;
	int collision_mask;
	float speed;
	float linear_acceleration;
	float curve_rate;
	float sin_amplitude;
	float sin_frequency;
	bool face_direction;
	Vector2 scale;

protected:
	static void _bind_methods();

public:
	BulletData();
	~BulletData();

	void set_texture(const Ref<Texture> &p_texture);
	Ref<Texture> get_texture() const;

	void set_material(const Ref<Material> &p_material);
	Ref<Material> get_material() const;

	void set_damage(float p_amount);
	float get_damage() const;

	void set_collision_shape(const Ref<Shape2D> &p_shape);
	Ref<Shape2D> get_collision_shape() const;

	void set_collision_mask(int p_mask);
	int get_collision_mask() const;

	void set_speed(float p_speed);
	float get_speed() const;

	void set_linear_acceleration(float p_acceleration);
	float get_linear_acceleration() const;

	void set_curve_rate(float p_degrees_per_sec);
	float get_curve_rate() const;

	void set_sin_amplitude(float p_amplitude);
	float get_sin_amplitude() const;

	void set_sin_frequency(float p_freq);
	float get_sin_frequency() const;

	void set_face_direction(bool p_enabled);
	bool get_face_direction() const;

	void set_scale(Vector2 p_scale);
	Vector2 get_scale() const;
};

#endif