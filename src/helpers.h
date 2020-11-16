//
// Created by kaloka on 14/11/2020.
//

#ifndef OGLHANGMAN_HELPERS_H
#define OGLHANGMAN_HELPERS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <typeinfo>

namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height);
    void render();
}

namespace events {
    template <typename ...T>
    class functionWrapperBase {
    public:
        virtual void call(T ...args) = 0;
        virtual bool operator == (functionWrapperBase<T...>* other) = 0;
        virtual ~functionWrapperBase() = default;
    };

    template<>
    class functionWrapperBase<void> {
    public:
        virtual void call() = 0;
        virtual bool operator == (functionWrapperBase<void>* other) = 0;
        virtual ~functionWrapperBase() = default;
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

        ~functionWrapperFunc() override = default;

    private:
        void (*funcCallback)(T...);
    };

    template<>
    class functionWrapperFunc<void> : public functionWrapperBase<void> {
    public:
        explicit functionWrapperFunc(void (*func)()) :
                funcCallback(func)
        {}

        void call() override {
            funcCallback();
        }

        bool operator == (functionWrapperBase<void>* other) override {
            if (typeid(*this) != typeid(*other)) {
                return false;
            } else if (funcCallback != (dynamic_cast<functionWrapperFunc<void> *>(other)->funcCallback)) {
                return false;
            } else { return true; }
        }

        ~functionWrapperFunc() override = default;

    private:
        void (*funcCallback)();
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

        ~functionWrapperMember() override = default;

    private:
        U *inst;
        void (U::*memberCallback)(T...);
    };

    template<typename U>
    class functionWrapperMember<U, void> : functionWrapperBase<void> {
    public:
        functionWrapperMember(void (U::*func)(), U *inst) :
                inst(inst),
                memberCallback(func)
        {}

        void call() override {
            (inst->*memberCallback)();
        }

        bool operator == (functionWrapperBase<void>* other) override {
            if (typeid(*this) != typeid(*other)) {
                return false;
            } else {
                auto castedOther = dynamic_cast<functionWrapperMember<U, void> *>(other);
                if (inst != castedOther->inst || memberCallback != castedOther->memberCallback) {
                    return false;
                }
                return true;
            }
        }

    private:
        U *inst;
        void (U::*memberCallback)();

        ~functionWrapperMember() override = default;
    };

    template <typename ...T>
    functionWrapperBase<T...> *handler(void (*func)(T...)) {
        return new functionWrapperFunc<T...>(func);
    }
    template <typename U, typename ...T>
    functionWrapperBase<T...> *handler(void (U::*func)(T...), U *inst) {
        return new functionWrapperMember<U, T...>(func, inst);
    }
    functionWrapperBase<void> *handler(void (*func)()) {
        return new functionWrapperFunc<void>(func);
    }
    template <typename U>
    functionWrapperBase<void> *handler(void (U::*func)(), U *inst) {
        return new functionWrapperMember<U, void>(func, inst);
    }

    template <typename ...T>
    class event {
    public:
        event() : handlers() {};

        event & operator += (functionWrapperBase<T...> *handler) {
            handlers.push_back(handler);
            return *this;
        }

        event & operator += (void (*funcPtr)(T...)) {
            *this += handler(funcPtr);
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

        event & operator -= (void (*funcPtr)(T...)) {
            *this -= handler(funcPtr);
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
    template <>
    class event<void> {
    public:
        event() : handlers() {};

        event & operator += (functionWrapperBase<void> *handler) {
            handlers.push_back(handler);
            return *this;
        }

        event & operator += (void (*funcPtr)()) {
            *this += handler(funcPtr);
            return *this;
        }

        event & operator -= (functionWrapperBase<void> *handler) {
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

        event & operator -= (void (*funcPtr)()) {
            *this -= handler(funcPtr);
            return *this;
        }

        void operator () () {
            for (auto handler : handlers) {
                handler->call();
            }
        }
        ~event(){
            for (auto handler : handlers) {
                delete handler;
            }
        }

    private:
        std::vector<functionWrapperBase<void>*> handlers;
    };
}
using events::event;
namespace input {
    struct inputActionEventsHolder {
        static event<void> accept;
        static event<float, float> move4Axis;
        static event<void> reload;
        static event<char>  genericCharacterEvent;
    };
    struct inputHandler {
    public:
        static inputActionEventsHolder inputActionEvents;
        inputHandler();
        static void keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void characterCallback(GLFWwindow *window, unsigned int codepoint);

    private:
        static void closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void processGenericCharacter(unsigned int codepoint);
    };
    void setupGlfwInputCallbacks(GLFWwindow *window, input::inputHandler handler);
}

namespace shaders
{
    constexpr const char* vertshader_loc = "vertshader.vert";
    constexpr const char* fragshader_loc = "fragshader.frag";

    unsigned int load_shader(const char* filename, int shadertype);
    unsigned int shader_program();
}

namespace sprites
{
    class Sprite
    {
    public:
        float x;
        float y;
        float w;
        float h;

        virtual void draw() = 0;
    };

    class Text: public Sprite
    {
    public:

        static unsigned int textids[100];
        static bool initflag;

        static unsigned int VBO, VAO, EBO;

        static void TextInit();
        
        Text();

        Text(std::string text, float posx = 0, float posy = 0, float height = 0.05);
        void setText(std::string newtext);
        std::string getText();

        void draw();

    private:
        std::string text = "";

    };

};


#endif //OGLHANGMAN_HELPERS_H
