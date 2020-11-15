//
// Created by kaloka on 14/11/2020.
//

#ifndef OGLHANGMAN_HELPERS_H
#define OGLHANGMAN_HELPERS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vector"
namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height);
    void render();
}

namespace input {
    void setupGlfwInputCallbacks(GLFWwindow *window);
    void closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
}

namespace events {
    template <typename ...T>
    class functionWrapperBase {
    public:
        virtual void call(T ...args) = 0;
        virtual bool operator == (functionWrapperBase<T...>* other) = 0;
    };

    template <typename ...T>
    class functionWrapperFunc : public functionWrapperBase<T...> {
    public:
        explicit functionWrapperFunc(void (*func)(T...)) :
        funcCallback(func)
        {}

        void call(T ...args) override {
            funcCallback(args...);
        }

        bool operator == (functionWrapperBase<T...>* other) override {
            if (typeid(*this) != typeid(*other)) {
                return false;
            } else if (funcCallback != (dynamic_cast<functionWrapperFunc<T...> *>(other)->funcCallback)) {
                return false;
            } else { return true; }
        }

    private:
        void (*funcCallback)(T...);
    };

    template <typename U, typename ...T>
    class functionWrapperMember : public functionWrapperBase<T...> {
    public:
        functionWrapperMember(void (U::*func)(T...), U *inst) :
        inst(inst),
        memberCallback(func)
        {}

        void call(T... args) override {
            (inst->*memberCallback)(args...);
        }

        bool operator == (functionWrapperBase<T...>* other) override {
            if (typeid(*this) != typeid(*other)) {
                return false;
            } else {
                auto castedOther = dynamic_cast<functionWrapperMember<U, T...> *>(other);
                if (inst != castedOther->inst || memberCallback != castedOther->memberCallback) {
                    return false;
                }
                return true;
            }
        }

    private:
        U *inst;
        void (U::*memberCallback)(T...);
    };

    template <typename ...T>
    functionWrapperBase<T...> *handler(void (*func)(T...)) {
        return new functionWrapperFunc<T...>(func);
    }
    template <typename U, typename ...T>
    functionWrapperBase<T...> *handler(void (U::*func)(T...), U *inst) {
        return new functionWrapperMember<U, T...>(func, inst);
    }

    template <typename ...T>
    class event {
    public:
        event() : handlers() {};

        event & operator += (functionWrapperBase<T...> *handler) {
            handlers.push_back(handler);
            return *this;
        }

        event & operator -= (functionWrapperBase<T...> *handler) {
            auto i = handlers.begin();
            auto end = handlers.end(); // cuz we modify
            for (; i != end; i++) {
                if (**i == handler) {
                    delete *i;
                    handlers.erase(i);
                    delete handler;
                    break;
                }
            }
            return *this;
        }

        void operator () (T ...args) {
            for (auto handler : handlers) {
                handler->call(args...);
            }
        }
        ~event(){
            for (auto handler : handlers) {
                delete handler;
            }
        }

    private:
        std::vector<functionWrapperBase<T...>*> handlers;

    };
}

namespace shaders
{
    unsigned int load_shader(const char* filename, int shadertype);
}


#endif //OGLHANGMAN_HELPERS_H
