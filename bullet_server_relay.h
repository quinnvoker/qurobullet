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

	void on_bullet_fired(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction);
	void on_volley_fired(const Ref<BulletType> &p_type, const Vector2 &p_position, const Array &p_shots);
};

#endif