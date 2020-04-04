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
	int intersect_shape(Physics2DDirectSpaceState &p_space_state,  Physics2DDirectSpaceState::ShapeResult *r_results);

	void set_active(bool p_value);
	bool get_active() const;

	void set_lifetime(float p_value);
	float get_lifetime() const;

	void set_direction(const Vector2 &p_value);
	Vector2 get_direction() const;

	void set_type(const Ref<BulletType> &p_value);
	Ref<BulletType> get_type() const;
};

#endif