#ifndef BULLETSERVER_H
#define BULLETSERVER_H

#include "scene/2d/node_2d.h"
#include "bullet.h"
#include "bullet_type.h"
#include "core/os/os.h"
#include <vector>

class BulletServer : public Node2D {
	GDCLASS(BulletServer, Node2D);

public:
	enum AreaMode {
		VIEWPORT,
		MANUAL,
		INFINITE,
	};

private:
	int bullet_pool_size;

	bool pop_on_collide;
	float max_lifetime;

	Vector<Bullet *> live_bullets;
	Vector<Bullet *> dead_bullets;

	AreaMode play_area_mode;
	Rect2 play_area_rect;
	float play_area_margin;
	bool play_area_allow_incoming;

	bool relay_autoconnect;

	void _process_bullets(float delta);

	void _init_bullets();
	void _create_bullet();

	void _update_play_area();

protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _validate_property(PropertyInfo &property) const;

public:
	BulletServer();
	~BulletServer();

	void spawn_bullet(const Ref<BulletType> &p_type, const Vector2 &p_position, const Vector2 &p_direction);
	void spawn_volley(const Ref<BulletType> &p_type, const Vector2 &p_position, const Array &p_volley);

	void clear_bullets();
	int get_live_bullet_count();

	Array get_live_bullets();
	Array get_live_bullet_positions();

	void set_bullet_pool_size(int p_size);
	int get_bullet_pool_size() const;

	void set_pop_on_collide(bool p_enabled);
	bool get_pop_on_collide() const;

	void set_max_lifetime(float p_time);
	float get_max_lifetime() const;

	void set_play_area_mode(AreaMode p_mode);
	AreaMode get_play_area_mode() const;

	void set_play_area_rect(const Rect2 &p_rect);
	Rect2 get_play_area_rect() const;

	void set_play_area_margin(float p_margin);
	float get_play_area_margin() const;

	void set_play_area_allow_incoming(bool p_enabled);
	bool get_play_area_allow_incoming() const;

	void set_relay_autoconnect(bool p_enabled);
	bool get_relay_autoconnect() const;
};

VARIANT_ENUM_CAST(BulletServer::AreaMode)

#endif
