#ifndef _ACTION_HPP_
#define _ACTION_HPP_

class world;
class actor;

class action {
    public:
    virtual void on_start(world &w){}
    virtual void on_complete(world &w){}
};

class sync_action : public action {
    protected:
    actor *actor_;

    public:
    sync_action(actor &a) :
        actor_(&a)
    {}
    
    virtual void sync_on_complete(world &w){}
    void on_complete(world &w);
};

class chain_action : public action {
    protected:
    chain_action *next_action_;
    public:
    chain_action(chain_action &next) : next_action_(&next) {}
    chain_action() {}

    virtual void chain_on_complete(world &w){}
    void on_complete(world &w);
};

#endif
