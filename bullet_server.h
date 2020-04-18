#ifndef BULLETSERVER_H
#define BULLETSERVER_H

#include "scene/2d/node_2d.h"
#include "bullet.h"
#include "bullet_data.h"
#include "core/os/os.h"
#include <vector>

class BulletServer : public Node2D {
	GDCLASS(BulletServer, Node2D);

	int bullet_pool_size;
	Rect2 play_area;
	float play_area_margin;

	bool pop_on_collide;
	float max_lifetime;

	Vector<Bullet *> live_bullets;
	Vector<Bullet *> dead_bullets;

	void _ready();
	void _process_internal(float delta);
	void _physics_process_internal(float delta);

	void _init_bullets();
	void _create_bullet();

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	BulletServer();
	~BulletServer();

	void spawn_bullet(const Ref<BulletData> &p_type, const Vector2 &p_position, const Vector2 &p_direction);
	void spawn_volley(const Ref<BulletData> &p_type, const Vector2 &p_position, const Array &p_volley);

	void clear_bullets();

	void set_bullet_pool_size(int p_size);
	int get_bullet_pool_size() const;

	void set_play_area_margin(float p_margin);
	float get_play_area_margin() const;

	void set_pop_on_collide(bool p_enabled);
	bool get_pop_on_collide() const;

	void set_max_lifetime(float p_time);
	float get_max_lifetime() const;
};

#endif
