#pragma once
#include "voltomino.h"
#include "object_registry.h"
#include "game_object.h"
#include <array>
#include <algorithm>
#include <vector>
#include <random>
namespace Volt {
	const int arenaHeight = 20;
	const int arenaWidth = 10;

	class ArenaBorder : public GameObject {
	public:
		ArenaBorder() = default;
		ArenaBorder(const Transform& t, const Mesh& mesh, const Collider& collider, const RigidBody& rb)
			: GameObject(t, mesh, collider, rb, 1) {}
	};

	struct Arena {
		ArenaBorder top;
		ArenaBorder left;
		ArenaBorder right;
		ArenaBorder bottom;
		Arena() = default;
		Arena(ArenaBorder top, ArenaBorder left, ArenaBorder right, ArenaBorder bottom) :
			top(top), left(left), right(right), bottom(bottom) {
		}
	};

	class GameManager {
	private:
		ObjectRegistry& registry;
		std::vector<Voltomino*> placedPieces;
		std::array<Voltomino, 7> pieces;
		Voltomino* currentPiece = nullptr;
		Voltomino* nextPiece = nullptr;
		Arena arena;
		bool gameOver = false;

	public:
		int score = 0;

	private:
		void placeAtStart(Voltomino* piece);
		Voltomino* initPiece();
		void placeNewPiece();
		void initArena();
		ArenaBorder createVerticalBorder(int startX, int startY, int height);
		ArenaBorder createHorizontalBorder(int startX, int startY, int width);

	public:
		bool isGameOver() const { return gameOver; }
		GameManager(ObjectRegistry& registry);
		~GameManager();
		void update();
		void newGame();
	};
}