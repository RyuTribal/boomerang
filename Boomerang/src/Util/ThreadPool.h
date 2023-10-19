#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <future>
#include <queue>

namespace Boomerang
{

    class VoidThreadPool {
    private:
        int threads;
        std::vector<std::function<void()>> functions;

    public:
        VoidThreadPool(int threads) : threads(threads) {}

        void submit_task(std::function<void()> function) {
            functions.push_back(function);
        }

        void run_tasks() {
            std::vector<std::future<void>> futures;
            for (const auto& func : functions) {
                futures.push_back(std::async(std::launch::async, func));
            }

            for (auto& future : futures) {
                try {
                    future.get();
                }
                catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }

        void clear()
        {
            functions.clear();
        }
    };

    template <typename R>
    class ReturningThreadPool {

    public:
        ReturningThreadPool(int threads) : threads(threads) {}

        void submit_task(std::function<R()> function) {
            functions.push_back(function);
        }

        std::shared_ptr<std::vector<R>> run_tasks() {
            std::vector<std::future<R>> futures;
            for (const auto& func : functions) {
                futures.push_back(std::async(std::launch::async, func));
            }

            std::vector<R> results;
            for (auto& future : futures) {
                try {
                    results.push_back(future.get());
                }
                catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            return std::make_shared<std::vector<R>>(results);
        }

        void clear()
        {
            functions.clear();
        }
    private:
        int threads;
        std::vector<std::function<R()>> functions;
    };

    template <typename R>
    class ThreadPoolBase
	{
    public:
        ThreadPoolBase(int threads) : pool{ threads } { }

        void submit_task(std::function<R()> function) {
            pool.submit_task(function);
        }

        std::shared_ptr< std::vector<R>> run_tasks() {
            return pool.run_tasks();
        }

        void clear()
        {
            pool.clear();
        }
    private:
        ReturningThreadPool<R> pool;
    };

    template <>
    class ThreadPoolBase<void>
	{
    public:
        ThreadPoolBase(int threads) : pool{ threads } { }

        void submit_task(std::function<void()> function) {
            pool.submit_task(function);
        }

        void run_tasks() {
            return pool.run_tasks();
        }

        void clear()
        {
            pool.clear();
        }
    private:
        VoidThreadPool pool;
    };

    template <typename R = void>
    using ThreadPool = ThreadPoolBase<R>;
}