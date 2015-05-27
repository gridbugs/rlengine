#ifndef _ACTOR_DRAWING_INTERFACE_
#define _ACTOR_DRAWING_INTERFACE_

template <typename C, typename K> class actor_drawing_interface {
    public:
    virtual const grid<K> &get_knowledge_grid() const = 0;
    virtual const C &get_character() const = 0;
};

#endif
