#include "../common_includes.hpp"

f_aim* cs_aim = new f_aim();

int f_aim::crosshair_id(vec3 vangle, cs_player self)
{
	vec3 dir, eye;
	int i;
	cs_player entity;
	int id, j;
	matrix3x4_t matrix;
	bool status;

	dir = u_math::vec_atd(vangle);
	eye = self.GetEyePos();

	for (i = 1; i < engine::GetMaxClients(); i++)
	{
		entity = entity::GetClientEntity(i);

		if (!entity.IsValid())
			continue;

		id = entity.GetTeam();

		if (!cvar::find("mp_teammates_are_enemies").GetInt() && self.GetTeam() == id)
			continue;

		id -= 2;

		for (j = 6; j--;)
		{
			entity.GetBoneMatrix(hitbox_list[id][j].bone, &matrix);
			status = u_math::vec_min_max(
				eye,
				dir,
				u_math::vec_transform(hitbox_list[id][j].min, matrix),
				u_math::vec_transform(hitbox_list[id][j].max, matrix),
				hitbox_list[id][j].radius
			);

			if (status)
				return i;
		}
	}
	return 0;
}

float f_aim::get_fov(vec3 vangle, vec3 angle)
{
	vec3 a0, a1;

	u_math::angle_vec(vangle, &a0);
	u_math::angle_vec(angle, &a1);

	return RAD2DEG(acos(u_math::vec_dot(a0, a1) / u_math::vec_length(a0)));
}

bool f_aim::get_target(cs_player self, vec3 vangle)
{
	float fov, best_fov = 9999.0f;
	int i;
	cs_player entity;

	for (i = 1; i < engine::GetMaxClients(); i++)
	{
		entity = entity::GetClientEntity(i);

		if (!entity.IsValid())
			continue;

		if (!cvar::find("mp_teammates_are_enemies").GetInt() && self.GetTeam() == entity.GetTeam())
			continue;

		fov = get_fov(vangle, get_target_angle(self, entity));

		if (fov < best_fov)
		{
			best_fov = fov;
			target = entity;
		}
	}
	return best_fov != 9999.0f;
}

void mouse_move(int x, int y)
{
	mouse_event(0x0001, x, y, 0, 0);
}

void mouse1_down(void)
{
	mouse_event(0x0002, 0, 0, 0, 0);
}

void mouse1_up(void)
{
	mouse_event(0x0004, 0, 0, 0, 0);
}

void f_aim::aim_at_target(vec3 vangle, vec3 angle)
{
	float x, y, sx, sy;

	y = vangle.x - angle.x;
	x = vangle.y - angle.y;

	if (y > 89.0f)   
		y = 89.0f;   
	else if (y < -89.0f)  
		y = -89.0f;

	if (x > 180.0f)
		x -= 360.0f;
	else if (x < -180.0f)
		x += 360.0f;

	if (fabs(x) / 180.0f >= cs_config->aimbot.fov)
		return;

	if (fabs(y) / 89.0f >= cs_config->aimbot.fov)
		return;

	x = ((x / flsensitivity) / 0.022f);
	y = ((y / flsensitivity) / -0.022f);

	if (cs_config->aimbot.smooth)
	{
		sx = 0.0f, sy = 0.0f;

		if (sx < x)
			sx += 1.0f + (x / cs_config->aimbot.smooth);
		else if (sx > x)
			sx -= 1.0f - (x / cs_config->aimbot.smooth);

		if (sy < y)
			sy += 1.0f + (y / cs_config->aimbot.smooth);
		else if (sy > y)
			sy -= 1.0f - (y / cs_config->aimbot.smooth);
	}
	else
	{
		sx = x;
		sy = y;
	}

	if (current_tick - previous_tick > 0) 
	{
		previous_tick = current_tick;
		mouse_move((int)sx, (int)sy);
	}
}

vec3 f_aim::get_target_angle(cs_player self, cs_player target)
{
	matrix3x4_t m;
	vec3 c, p;

	target.GetBoneMatrix(8, &m);
	c = self.GetEyePos();

	m[0][3] -= c.x, m[1][3] -= c.y, m[2][3] -= c.z;
	c.x = m[0][3], c.y = m[1][3], c.z = m[2][3];

	u_math::vec_normalize(&c);
	u_math::vec_angles(c, &c);

	if (self.GetShotsFired() > 1)
	{
		p = self.GetVecPunch();
		c.x -= p.x * 2.0f, c.y -= p.y * 2.0f, c.z -= p.z * 2.0f;
	}

	u_math::vec_clamp(&c);

	return c;
}


void f_aim::triggerbot()
{
	cs_player self;
	vec3 vangle;

	while (engine::IsRunning())
	{
		if (engine::IsInGame())
		{
			if (triggerbot_enabled && inputsystem::IsButtonDown(cs_config->triggerbot.button))
			{
				self = entity::GetClientEntity(engine::GetLocalPlayer());
				vangle = engine::GetViewAngles();

				if (crosshair_id(vangle, self))
				{
					mouse1_down();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					mouse1_up();
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void f_aim::aimbot()
{
	cs_player self;
	vec3 vangle;

	while (engine::IsRunning())
	{
		if (engine::IsInGame())
		{
			self = entity::GetClientEntity(engine::GetLocalPlayer());
			vangle = engine::GetViewAngles();
			current_tick = self.GetTickCount();
			flsensitivity = cvar::find("sensitivity").GetFloat();

			if (self.IsScoped())
				flsensitivity = (self.GetFov() / 90.0f) * flsensitivity;

			if (aimbot_enabled && inputsystem::IsButtonDown(cs_config->aimbot.button))
			{
				if (!target.IsValid() && !get_target(self, vangle))
					return;

				aim_at_target(vangle, get_target_angle(self, target));
			}
			else
			{
				target = {};
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}