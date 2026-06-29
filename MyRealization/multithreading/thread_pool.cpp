#include <thread>				// For std::thread
#include <queue>				// For std::queue
#include <functional>			// For std::functional, std::bind
#include <mutex>				// For std::mutex, std::once_flag, std::call_once, std::unique_lock, std::lock_guard
#include <condition_variable>	// For std::condition_variable
#include <atomic>				// For std::atomic
#include <cstddef>				// For std::size_t
#include <future>				// For std::future, std::packaged_task
#include <type_traits>			// For std::invoke_result
#include <stdexcept>			// For std::runtime_error
#include <memory>				// For std::shared_ptr, std::make_shared
#include <utility>				// For std::forward
#include <vector> 				// For std::vector


class ThreadPool 
{
private:

	std::vector<std::thread> workers_;
	std::queue<std::function<void()>> tasks_;

	std::mutex q_mtx_;
	std::condition_variable q_cv_;

	std::atomic<bool> is_joined_;
	std::once_flag join_flag_;
	
	// Worker-thread body
	void do_work()
	{
		while (true)
		{
			std::function<void()> current_task;
			{
				std::unique_lock<std::mutex> lock(q_mtx_);

				q_cv_.wait(lock, [this]() {
					return !tasks_.empty() || is_joined_;
					});

				if (is_joined_ && tasks_.empty())
					return;

				current_task = std::move(tasks_.front());
				tasks_.pop();
			}
			current_task();
		}
	}

	// Adding task f(args...) and return std::future object
	template<typename Func, typename... Args>
	std::future<typename std::invoke_result<Func, Args...>::type> add_(Func&& f, Args&&... args)
	{
		if (is_joined_)
			throw std::runtime_error("add_task on joined ThreadPool");

		using ret_type = typename std::invoke_result<Func, Args...>::type;
		using pack_task = std::packaged_task<ret_type()>;

		std::shared_ptr<pack_task> task_ptr = std::make_shared<pack_task>(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

		std::future<ret_type> future = task_ptr->get_future();
		{
			std::lock_guard<std::mutex> lock(q_mtx_);
			if (is_joined_)
				throw std::runtime_error("add_task on joined ThreadPool");

			tasks_.emplace([task_ptr]() { (*task_ptr)(); });
		}

		q_cv_.notify_one();

		return future;
	}

public:

	explicit ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency())
		: is_joined_(false)
	{
		num_threads = (num_threads < 1 ? 1 : num_threads);
	
		std::size_t i{};
		for (; i < num_threads; ++i)
			workers_.emplace_back(&ThreadPool::do_work, this);
	}

	// Task with one-thread access
	template<typename Func, typename... Args>
	std::future<typename std::invoke_result<Func, Args...>::type> add_task(Func&& f, Args&& ...args)
	{
		return add_(std::forward<Func>(f), std::forward<Args>(args)...);
	}

	// Task with shared access
	template<typename Func, typename... Args>
	std::shared_future<typename std::invoke_result<Func, Args...>::type> add_task_shared(Func&& f, Args&& ...args)
	{
		std::future<typename std::invoke_result<Func, Args...>::type> future = add_(std::forward<Func>(f), std::forward<Args>(args)...);
		return future.share();
	}

	// "close" threadpool. Can be called once
	void join()
	{
		std::call_once(join_flag_, [this]() {
			is_joined_ = true;
			q_cv_.notify_all();

			for (std::thread& worker : workers_)
				if (worker.joinable())
					worker.join();
		});
	}

	~ThreadPool()
	{
		join();
	}

};
