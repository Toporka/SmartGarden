#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Signal
{
private:
    std::vector<std::function<void(Args...)>> listFunctions;
public:
    void connect(std::function<void(Args...)> slot)
    {
        listFunctions.push_back(slot);
    }

    template<typename T, typename Ret>
    void connect(T* obj, Ret(T::* method)(Args...))
    {
        listFunctions.push_back([obj, method](Args... args) {
            (obj->*method)(args...);
        });
    }

    template<typename T, typename Ret>
    void connect(const T* obj, Ret(T::* method)(Args...) const)
    {
        listFunctions.push_back([obj, method](Args... args) {
            (obj->*method)(args...);
        });
    }

    void emit(Args... args)
    {
        for (auto& item : listFunctions)
        {
            item(args...);
        }
    }
};