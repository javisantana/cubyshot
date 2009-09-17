

void final_boss_update(struct actor_t*, float);
void final_boss_render(const struct actor_t*);
void final_boss_init(struct actor_t* a);
bool final_boss_collide(struct actor_t* a, struct actor_t* bullet);