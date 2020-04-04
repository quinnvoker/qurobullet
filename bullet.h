#ifndef BULLET_H
#define BULLET_H

#include "bullet_type.h"
#include "scene/2d/node_2d.h"
#include "servers/physics_2d_server.h"

class Bullet : public Node2D {
	GDCLASS(Bullet, Node2D);

private:
	bool active;
	float lifetime;

	Vector2 direction;
	Vector2 perp_offset;

	Ref<BulletType> type;

protected:
	void _notification(int p_what);

	static void _bind_methods();

public:
	Bullet();
	~Bullet();

	void _ready();

	void move(float delta);
	int intersect_shape(Physics2DDirectSpaceState &p_space_state, Physics2DDirectSpaceState::ShapeResult *r_results);

	void set_active(bool value);
	bool get_active();

	void set_lifetime(float value);
	float get_lifetime();

	void set_direction(Vector2 value);
	Vector2 get_direction();

	void set_type(Ref<BulletType> value);
	Ref<BulletType> get_type();
};

#endif