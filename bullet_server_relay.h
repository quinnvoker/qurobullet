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

	void on_bullet_fired(Ref<BulletType> p_type, Vector2 p_position, Vector2 p_direction);
	void on_volley_fired(Ref<BulletType> p_type, Vector2 p_position, Array shots);
};

#endif