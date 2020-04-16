#ifndef BULLET_H
#define BULLET_H

#include "core/object.h"
#include "core/reference.h"
#include "bullet_data.h"
#include "core/math/transform_2d.h"
#include "core/math/rect2.h"
#include "scene/resources/texture.h"


class Bullet : public Object {
	GDCLASS(Bullet, Object);

	float time;
	bool _popped;

	Ref<BulletData> data;

	Vector2 direction;
	Vector2 position;
	float rotation;
	Vector2 _offset;

	RID ci_rid;

	void _update_offset();

	protected:
		static void _bind_methods();

	public:
		void spawn(const Ref<BulletData> &p_data, const Vector2 &p_position, const Vector2 &p_direction);
		
		void update(float delta);

		void pop();
		bool is_popped();

		bool can_collide();

		void set_time(float p_time);
		float get_time() const;

		void set_data(const Ref<BulletData> &p_data);
		Ref<BulletData> get_data() const;

		void set_direction(const Vector2 &p_direction);
		Vector2 get_direction() const;

		void set_position(const Vector2 &p_position);
		Vector2 get_position() const;

		void set_rotation(float p_radians);
		float get_rotation() const;

		Transform2D get_transform();

		void set_ci_rid(const RID $p_rid);
		RID get_ci_rid() const;

		void ci_set_material(const Ref<Material> &p_material);
		void ci_draw_texture(const Ref<Texture> &p_texture);

		Bullet();
		~Bullet();
};

#endif