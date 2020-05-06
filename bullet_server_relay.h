#ifndef BULLETSERVERRELAY_H
#define BULLETSERVERRELAY_H

#include "bullet_type.h"
#include "core/object.h"

class BulletServerRelay : public Object {
	GDCLASS(BulletServerRelay, Object);

protected:
	static void _bind_methods();

public:
	BulletServerRelay();
	~BulletServerRelay();

	void spawn_bullet(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction);
	void spawn_volley(const Ref<BulletType> &p_type, const Vector2 &p_origin, const Array &p_shots);
};

#endif