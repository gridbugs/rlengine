#ifndef _ACTION_HPP_
#define _ACTION_HPP_


class world;
class actor;

class action {
    protected:
    bool active_;

    public:
    virtual void on_start(world &w) const {}
    virtual void apply(world &w) const {}
    virtual void on_complete(world &w) const {}
    virtual int get_duration() const {return 0;}

    action() : active_(true) {}
    void cancel() {active_ = false;}
    bool is_active() const {return active_;}
};

#endif
