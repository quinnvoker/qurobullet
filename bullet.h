#ifndef BULLET_H
#define BULLET_H

#include "bullet_type.h"
#include "scene/2d/node_2d.h"
#include "servers/physics_2d_server.h"

class Bullet : public Node2D {
	GDCLASS(Bullet, Node2D);

	bool active;
	float lifetime;
	bool _popped;

	Vector2 direction;
	Vector2 perp_offset;

	Ref<BulletType> type;

	void _ready();

protected:
	void _notification(int p_what);

	static void _bind_methods();

public:
	Bullet();
	~Bullet();

	void spawn(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction);

	void update_position(float delta);

	void pop();
	bool is_popped();

	void set_active(bool p_active);
	bool get_active() const;

	void set_lifetime(float p_time);
	float get_lifetime() const;

	void set_direction(const Vector2 &p_direction);
	Vector2 get_direction() const;

	void set_type(const Ref<BulletType> &p_type);
	Ref<BulletType> get_type() const;
};

#endif