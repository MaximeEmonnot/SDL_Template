#pragma once
#include <memory>

namespace CoreSystem {
	template <typename T>
	class SingletonMaker {
	public:
		static std::shared_ptr<T> GetInstance() {
			if (singleton == nullptr) {
				singleton = std::make_shared<T>();
			}
			return singleton;
		}
		template<typename Q>
		static std::shared_ptr<T> GetInstance(Q param) {
			if (singleton == nullptr) {
				singleton = std::make_shared<T>(param);
			}
			return singleton;
		}
		template <typename ...Args>
		static std::shared_ptr<T> GetInstance(Args... param) {
			if (singleton == nullptr) {
				singleton = std::make_shared<T>(param...);
			}
			return singleton;
		}
	private:
		static std::shared_ptr<T> singleton;
	};
	template <typename T> std::shared_ptr<T> SingletonMaker<T>::singleton = nullptr;
}