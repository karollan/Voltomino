#include "game_manager.h";

namespace Volt {

	static int randomPieceIndex() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, 6);
		return distr(gen);
	}

	GameManager::GameManager(ObjectRegistry& registry) : 
		registry(registry),
		pieces{
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{0,0},{0,1},{0,2},{1,2}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		},
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{1,0},{1,1},{1,2},{0,2}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		},
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{0,0},{0,1},{0,2},{0,3}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		},
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{0,0},{0,1},{1,0},{1,1}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		},
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{1,0},{0,1},{1,1},{2,1}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		},
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{1,0},{2,0},{0,1},{1,1}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		},
		Voltomino{
			Transform{ 0,0 },
			Mesh{ {{0,0},{1,0},{1,1},{2,1}}, '#' },
			Collider{ 0, 1 },
			RigidBody{ 0 }
		}
	}
	{
		initArena();
		placeNewPiece();
	}

	GameManager::~GameManager() {
		delete currentPiece;
		delete nextPiece;
		registry.UnregisterObject(static_cast<GameObject*>(currentPiece));
		registry.UnregisterObject(static_cast<GameObject*>(nextPiece));
		for (Voltomino* piece : placedPieces) {
			delete piece;
		}
		placedPieces.clear();
	}

	void GameManager::newGame() {
		delete currentPiece;
		delete nextPiece;
		currentPiece = nullptr;
		nextPiece = nullptr;
		for (Voltomino* piece : placedPieces) {
			delete piece;
		}
		placedPieces.clear();

		for (GameObject* obj : registry.objects) {
			registry.UnregisterObject(obj);
		}
		registry.objects.clear();

		score = 0;
		initArena();
		placeNewPiece();
		gameOver = false;
	}

	void GameManager::initArena() {
		ArenaBorder topBorder = createHorizontalBorder(0, 0, arenaWidth + 1);
		ArenaBorder leftBorder = createVerticalBorder(0, 0, arenaHeight + 1);
		ArenaBorder rightBorder = createVerticalBorder(arenaWidth + 1, 0, arenaHeight + 1);
		ArenaBorder bottomBorder = createHorizontalBorder(0, arenaHeight + 1, arenaWidth + 2);
		arena = Arena{
			topBorder, leftBorder, rightBorder, bottomBorder
		};
		registry.RegisterObject(&arena.top);
		registry.RegisterObject(&arena.left);
		registry.RegisterObject(&arena.right);
		registry.RegisterObject(&arena.bottom);
	}

	ArenaBorder GameManager::createVerticalBorder(int startX, int startY, int height) {
		Transform borderTransform{ startX, startY };
		std::vector<Cell> verticalBorder;
		verticalBorder.reserve(height);
		for (std::size_t i = 0; i < height; ++i) {
			verticalBorder.emplace_back(0, i);
		}
		Mesh borderMesh{
			verticalBorder,
			'x'
		};
		Collider collider{ 0, 1 };
		RigidBody rb{ 0 };
		return ArenaBorder{ borderTransform, borderMesh, collider, rb };
	}

	ArenaBorder GameManager::createHorizontalBorder(int startX, int startY, int width) {
		Transform borderTransform{ startX, startY };
		std::vector<Cell> horizontalBorder;
		horizontalBorder.reserve(width);
		for (std::size_t i = 0; i < width; ++i) {
			horizontalBorder.emplace_back(i, 0);
		}
		Mesh borderMesh{
			horizontalBorder,
			'x'
		};
		Collider collider{ 0, 1 };
		RigidBody rb{ 0 };
		return ArenaBorder{ borderTransform, borderMesh, collider, rb };
	}

	void GameManager::placeAtStart(Voltomino* piece) {
		piece->transform.x = arenaWidth / 2;
		piece->transform.y = 1;
	}

	Voltomino* GameManager::initPiece() {
		int randomIndex = randomPieceIndex();
		Voltomino* piece = new Voltomino(pieces[randomIndex]);
		placeAtStart(piece);
		piece->onCollision = [piece, this](GameObject* against, CollisionSide side) {
			if (against == dynamic_cast<GameObject*>(&arena.right)
				&& CollisionSide::bottom) {
				return;
			}
			if (against == dynamic_cast<GameObject*>(&arena.left)
				&& CollisionSide::bottom) {
				return;
			}
			if (side == CollisionSide::bottom) {
				piece->rb.gravity = 0;
				piece->state = PLACED;
				piece->onInput = nullptr;
				piece->onCollision = nullptr;
				if (piece->transform.y == 1) {
					gameOver = true;
				}
				else {
					placedPieces.push_back(piece);
					placeNewPiece();
				}
			}
		};
		piece->state = READY;
		registry.RegisterObject(piece);
		return piece;
	}

	void GameManager::placeNewPiece() {
		if (nextPiece != nullptr) {
			currentPiece = nextPiece;
			placeAtStart(currentPiece);
			nextPiece = initPiece();
		}
		else {
			currentPiece = initPiece();
			nextPiece = initPiece();
		}
		nextPiece->transform.x = arenaWidth * 1.5;
		nextPiece->transform.y = arenaHeight * 0.25;

		currentPiece->onInput = [this](WORD key) {
			if (key == 39) {
				currentPiece->rb.deltaX = 1;
			}
			if (key == 37) {
				currentPiece->rb.deltaX = -1;
			}
			if (key == 38) {
				currentPiece->rb.rotationDelta = 1;
			}
			if (key == 40) {
				currentPiece->rb.deltaY += 1;
			}
		};
	}

	void GameManager::update() {
		if (currentPiece != nullptr) {
			switch (currentPiece->state) {
			case READY:
				currentPiece->rb.gravity = 1;
				currentPiece->state = PLAYING;
				break;
			}
		}

		if (!placedPieces.empty()) {
			const int arraySize = arenaWidth * arenaHeight;
			std::array<int, arraySize> hits;
			hits.fill(0);
			for (const auto& piece : placedPieces) {
				std::vector<Cell> cells = piece->getCellsAt(piece->transform.x, piece->transform.y);
				for (const auto& cell : cells) {
					for (int i = 0; i < arenaHeight; ++i) {
						if (cell.y == arenaHeight - i) {
							hits.at(i * arenaWidth + (cell.x - 1)) = 1;
						}
					}
				}
			}

			std::array<int, arenaHeight> rows;
			rows.fill(0);
			int countRow = 0;
			for (int i = 0; i < arenaHeight; ++i) {
				for (int j = 0; j < arenaWidth; ++j) {
					if (hits.at(i * arenaWidth + j) == 1) {
						countRow += 1;
					}
				}
				if (countRow == arenaWidth) {
					rows[i] = 1;
				}
				countRow = 0;
			}

			for (int i = 0; i < arenaHeight; ++i) {
				if (rows.at(i) == 0) continue;

				for (auto it = placedPieces.begin(); it != placedPieces.end();) {
					Voltomino* piece = *it;

					std::erase_if(piece->mesh.cells, [&piece, i](const Cell& c) {
						return (arenaHeight - (c.y + piece->transform.y)) == i;
						});
					if (piece->mesh.cells.empty()) {
						delete piece;
						it = placedPieces.erase(it);
						auto registeredPiece = std::find(registry.objects.begin(), registry.objects.end(),
							static_cast<GameObject*>(piece));
						if (registeredPiece != registry.objects.end()) {
							registry.objects.erase(registeredPiece);
						}
					}
					else {
						for (auto& cell : piece->mesh.cells) {
							if (cell.y + piece->transform.y > (arenaHeight - i)) {
								cell.y = cell.y - 1;
							}
						}
						++it;
					}
				}
				score += 100;
			}
			for (int i = 0; i < arenaHeight; ++i) {
				if (rows.at(i) == 1) {
					for (const auto& piece : placedPieces) {
						if (piece->transform.y > i) {
							piece->transform.y += 1;
						}
					}
				}
			}
		}
	}
}