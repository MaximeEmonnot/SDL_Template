#pragma once
#include <memory>

namespace CoreSystem {
	template <typename T>
	class SingletonMaker {
	public:
		static std::shared_ptr<T> GetInstance() {
			if (mspSingleton == nullptr) {
				mspSingleton = std::make_shared<T>();
			}
			return mspSingleton;
		}
		template <typename ...Args>
		static std::shared_ptr<T> GetInstance(Args... param) {
			if (mspSingleton == nullptr) {
				mspSingleton = std::make_shared<T>(param...);
			}
			return mspSingleton;
		}

		static void Kill() {
			mspSingleton = nullptr;
		}

	private:
		static std::shared_ptr<T> mspSingleton;
	};
	template <typename T> std::shared_ptr<T> SingletonMaker<T>::mspSingleton = nullptr;
}