#ifndef EVENT_HPP
#define EVENT_HPP

enum class Event {
    NONE,
    CONFIRM,
    CANCEL,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    MENU,
    MOUSEMOVE,
    QUIT,
    NEXT,
    PREVIOUS,
    WINDOW_RESIZE,
    OTHER
};

#endif // EVENT_HPP
