#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "schedule/schedule.hpp"
#include "world/world.hpp"

template <typename C, typename W, typename T> class actor : public schedule_callback<world<C, W, T>> {};

#endif
