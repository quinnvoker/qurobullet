#ifndef BULLETSERVER_H
#define BULLETSERVER_H

#include "bullet.h"
#include "bullet_type.h"
#include "core/os/os.h"
#include <vector>

class BulletServer : public Node2D {
	GDCLASS(BulletServer, Node2D);

private:
	int bullet_pool_size;
	Rect2 play_area;
	int play_area_margin;

	std::vector<Bullet *> live_bullets;
	std::vector<Bullet *> dead_bullets;

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	BulletServer();
	~BulletServer();

	void _ready();

	void _physics_process(float delta);

	void spawn_bullet(Ref<BulletType> type, Vector2 position, Vector2 direction);
	void spawn_volley(Ref<BulletType> type, Vector2 position, Array shots);

	void kill_bullet(Bullet *bullet);

	void _init_bullets();
	void _create_bullet();

	void set_bullet_pool_size(int value);
	int get_bullet_pool_size();

	void set_play_area_margin(int value);
	int get_play_area_margin();
};

#endif
