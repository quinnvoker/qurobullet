#ifndef BULLETSERVER_H
#define BULLETSERVER_H

#include "bullet.h"
#include "bullet_type.h"
#include "core/os/os.h"
#include <vector>

class BulletServer : public Node2D {
	GDCLASS(BulletServer, Node2D);

	int bullet_pool_size;
	Rect2 play_area;
	float play_area_margin;

	bool auto_pop;

	std::vector<Bullet *> live_bullets;
	std::vector<Bullet *> dead_bullets;

	void _ready();
	void _physics_process(float delta);

	void _init_bullets();
	void _create_bullet();

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	BulletServer();
	~BulletServer();

	void spawn_bullet(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction);
	void spawn_volley(const Ref<BulletType> &p_type, const Vector2 &p_position, const Array &p_shots);

	void set_bullet_pool_size(int p_size);
	int get_bullet_pool_size() const;

	void set_play_area_margin(float p_margin);
	float get_play_area_margin() const;

	void set_auto_pop(bool p_enabled);
	bool get_auto_pop() const;
};

#endif
