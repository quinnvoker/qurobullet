#ifndef BULLET_H
#define BULLET_H

#include "core/object.h"
#include "core/reference.h"
#include "bullet_type.h"
#include "core/math/transform_2d.h"
#include "core/math/rect2.h"
#include "scene/resources/texture.h"


class Bullet : public Object {
	GDCLASS(Bullet, Object);

	float time;
	bool _popped;

	Ref<BulletType> type;

	Vector2 direction;
	Vector2 position;
	float rotation;
	Vector2 _offset;

	RID ci_rid;

	void _update_offset();
	void _update_appearance(const Ref<BulletType> &p_type = NULL);

	protected:
		static void _bind_methods();

	public:
		void spawn(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction);
		
		void update(float delta);

		void pop();
		bool is_popped();

		bool can_collide();

		void set_time(float p_time);
		float get_time() const;

		void set_type(const Ref<BulletType> &p_type);
		Ref<BulletType> get_type() const;

		void set_direction(const Vector2 &p_direction);
		Vector2 get_direction() const;

		void set_position(const Vector2 &p_position);
		Vector2 get_position() const;

		void set_rotation(float p_radians);
		float get_rotation() const;

		Transform2D get_transform();

		void set_ci_rid(const RID &p_rid);
		RID get_ci_rid() const;

		Bullet();
		~Bullet();
};

#endif