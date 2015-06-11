#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "schedule/schedule.hpp"
#include "world/world.hpp"

class actor : public schedule_callback<world> {};

#endif
