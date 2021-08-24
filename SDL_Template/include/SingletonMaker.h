#pragma once
#include <memory>

namespace CoreSystem {
	template <typename T>
	class SingletonMaker {
	public: 
		class Deleter {
		public:
			void operator()(T* p) {
				delete p;
			}
		};
	public:
		SingletonMaker() = default;
		~SingletonMaker() = default;
		SingletonMaker(const SingletonMaker&) = delete;
		SingletonMaker(SingletonMaker&&) = delete;
		SingletonMaker& operator=(const SingletonMaker&) = delete;
		SingletonMaker& operator=(SingletonMaker&&) = delete;

		static inline T& GetInstance() {
			if (mspSingleton == nullptr) {
				mspSingleton = std::make_unique<T>();
			}
			return *mspSingleton;
		}
		template <typename ...Args>
		static inline T& GetInstance(Args... param) {
			if (mspSingleton == nullptr) {
				mspSingleton = std::make_unique<T>(param...);
			}
			return *mspSingleton;
		}

		static inline void Kill() {
			mspSingleton = nullptr;
		}

	private:
		static std::unique_ptr<T> mspSingleton;
	};
	template <typename T> std::unique_ptr<T> SingletonMaker<T>::mspSingleton = nullptr;
}
