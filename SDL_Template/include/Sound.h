#pragma once
#include <SDL_mixer.h>
#include <string>
#include <memory>
#include "EngineException.h"

namespace SoundEngine {
	class ASound {
	public:
		class Hash {
		public:
			size_t operator()(const ASound& sound) const {
				size_t hash = 0;
				for (int i = 0; i < sound.path.size(); ++i) {
					hash += sound.path[i];
					hash += (hash << 10);
					hash ^= (hash >> 6);
				}
				hash += (hash << 3);
				hash ^= (hash >> 11);
				hash += (hash << 15);
				return hash;
			}
			size_t operator()(const std::unique_ptr<ASound>& sound) const {
				size_t hash = 0;
				for (int i = 0; i < sound->path.size(); ++i) {
					hash += sound->path[i];
					hash += (hash << 10);
					hash ^= (hash >> 6);
				}
				hash += (hash << 3);
				hash ^= (hash >> 11);
				hash += (hash << 15);
				return hash;
			}
			size_t operator()(const std::shared_ptr<ASound>& sound) const {
				size_t hash = 0;
				for (int i = 0; i < sound->path.size(); ++i) {
					hash += sound->path[i];
					hash += (hash << 10);
					hash ^= (hash >> 6);
				}
				hash += (hash << 3);
				hash ^= (hash >> 11);
				hash += (hash << 15);
				return hash;
			}
		};
	public:
		virtual void Play(int loops) = 0;
		virtual void Stop() = 0;

		inline bool operator==(const ASound& rhs) const {
			return path == rhs.path;
		}
		inline std::string	GetPath() const {
			return path;
		}
	protected:
		std::string			path = "";
	};
}