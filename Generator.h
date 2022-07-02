#pragma once

//import std.core;
#include <coroutine>
#include <exception>
#include <optional>
#include <iostream>

template<typename T>
struct Generator {
    //struct promise_type;
    //using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {// required 
        std::optional<T> value{};
        std::exception_ptr exception_;

        Generator get_return_object() {
            return Generator(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {
            //std::rethrow_exception(std::move(std::current_exception()));
            exception_ = std::current_exception(); //saving exception
        }
                //template<std::convertible_to<T> From> // C++20 concept
                //std::suspend_always yield_value(From&& from) {
                //    value = std::forward<From>(from);//caching the result in promise
                //    return {};
                //}
        //template<std::convertible_to<T> From> // C++20 concept
        std::suspend_always yield_value(T t) {
            value = std::move(t);
            return {};
        }
        //template<std::convertible_to<T> From> // C++20 concept
        //void return_value(From&& from) {
        //    value = std::forward<From>(from);//caching the result in promise
        //}
        //void return_value(T t) {
        //    value = std::move(t);
        //}
        void return_void() {
            value = std::nullopt;
        }

        bool finished() {
            return !value.has_value();
        }
    };

    //std::coroutine_handle<promise_type> handle;

    Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { /*if (handle)*/ handle.destroy(); }


    //explicit operator bool() {
    //    fill();// The only way to reliably find out whether or not we finished coroutine, 
    //           // whether or not there is going to be a next value generated (co_yield) in coroutine
    //           // via C++ getter (operator () below) 
    //           // is to execute/resume coroutine until the next co_yield point (or let it fall off end).
    //           // Then we store/cache result in promise to allow getter (operator() below to grab it 
    //           // without executing coroutine)
    //    return !handle.done();
    //}
    //T operator()() {
    //    fill();
    //    full_ = false;//we are going to move out previously cached result to make promise empty again
    //    return std::move(handle.promise().value);
    //}

    struct end_iterator {};

    class iterator
    {
    public:
        using value_type = std::optional<T>;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        iterator() = default;
        iterator(Generator& generator) : generator{ &generator }
        {}

        value_type operator*() const {
            if (generator) {
                return generator->handle.promise().get_value();
            }
            return {};
        }

        value_type operator->() const {
            if (generator) {
                return generator->handle.promise().get_value();
            }
            return {};
        }

        iterator& operator++() {
            if (generator && generator->handle) {
                generator->handle.resume();
            }
            return *this;
        }

        iterator& operator++(int) {
            if (generator && generator->handle) {
                generator->handle.resume();
            }
            return *this;
        }

        bool operator== (const end_iterator&) const {
            return generator ? generator->handle.promise().finished() : true;
        }

    private:
        Generator* generator{};
    };

    iterator begin() {
        iterator it{ *this };
        return ++it;
    }

    end_iterator end() {
        return end_sentinel;
    }

private:
    end_iterator end_sentinel{};
    std::coroutine_handle<promise_type> handle;

//private:
//    //bool full_ = false;
//
//    void fill() {
//        if (!full_) {
//            handle();
//            if (handle.promise().exception_)
//                std::rethrow_exception(handle.promise().exception_);
//            //propagate coroutine exception in called context
//
//            full_ = true;
//        }
//    }
};

namespace STL {

struct Plane {
    double cx, cy, cz, cc;
    double dot(const Vertex& v) const { return cx * v.x + cy * v.y + cz * v.z + cc; }
};

Generator<Facet> cut(const Facet& f, const Plane& p)
{
    // +| 0000111223
    // 0| 0123012010
    // -| 3210210100
    //    KKKKSCDCDD    Keep/Split/Cut/Drop
                            // - (<=0)*3 => Keep
                            // - (>=0)*3 => Drop
                            // - (>=0)*2 [(<0)*1] => Cut
                            // - [(<0)*2 (>0)*1] => Split
    double dots[3];
    for (int i = 0; i < 3; ++i)
        dots[i] = p.dot(f.vertices[i]);

    const int first_strict_positive = dots[0] > 0 ? 0 : dots[1] > 0 ? 1 : dots[2] > 0 ? 2 : 3;
    const int first_strict_negative = dots[0] < 0 ? 0 : dots[1] < 0 ? 1 : dots[2] < 0 ? 2 : 3;
    const int third_index = 3 - first_strict_positive - first_strict_negative;
    auto project = [points = f.vertices, &dots](int to_move, int target) {
        return Vertex{
            .x = static_cast<float>((dots[target] * points[to_move].x - dots[to_move] * points[target].x) / (dots[target] - dots[to_move])),
            .y = static_cast<float>((dots[target] * points[to_move].y - dots[to_move] * points[target].y) / (dots[target] - dots[to_move])),
            .z = static_cast<float>((dots[target] * points[to_move].z - dots[to_move] * points[target].z) / (dots[target] - dots[to_move]))
        };
    };

    if (first_strict_positive == 3) // Keep
    {
        //co_return f;
        co_yield f;
//        co_return;
    }
    else if (first_strict_negative == 3) { // Drop
//        co_return;
    }
    //auto project = [&fixed_point = &f.vertices[first_strict_negative], fixed_value = dots[first_strict_negative]](const Vertex& target, double target_value) {
    //    return Vertex{
    //        .x = (target_value * fixed_point.x - fixed_value * target.x) / (target_value - fixed_value);
    //        .y = (target_value * fixed_point.y - fixed_value * target.y) / (target_value - fixed_value);
    //        .z = (target_value * fixed_point.z - fixed_value * target.z) / (target_value - fixed_value);
    //    };
    //}
    else if (dots[third_index] >= 0) // Cut
    {
        // TODO respecter l'orientation
        Facet result;
        result.vertices[0] = f.vertices[first_strict_negative];
        result.vertices[1] = project(third_index, first_strict_negative);
        result.vertices[2] = project(first_strict_negative, first_strict_negative);
        co_yield result;
//        co_return;
        //co_return result;
        //co_return Facet{
        //    .vertices = {
        //        f.vertices[first_strict_negative],
        //        project(f.vertices[third_index], dots[third_index]),
        //        project(f.vertices[first_strict_negative], dots[first_strict_negative])
        //    };
        //};
    }
    else { // Split
        Vertex v = project(first_strict_positive, third_index);
        // TODO respecter l'orientation
        co_yield Facet{
            .vertices = {
                f.vertices[first_strict_negative],
                f.vertices[third_index],
                v
            }
        };
        // TODO respecter l'orientation
        co_yield Facet{
            .vertices = {
                f.vertices[first_strict_negative],
                v,
                project(first_strict_positive, first_strict_negative)
            }
        };
    }
    co_return;
}   


}

