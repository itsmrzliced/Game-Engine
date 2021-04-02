#include "Engine.h"

Engine::Engine(int width, int height, std::string title) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		std::cout << "SDL couldn't be initialized: " << SDL_GetError() << std::endl; 

		return;

	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {

		std::cout << "SDL_image couldn't be initialized: " << IMG_GetError() << std::endl; 

		return; 

	}

	if (TTF_Init() < 0) {

		std::cout << "SDL_ttf couldn't be initialized: " << TTF_GetError() << std::endl; 

		return; 

	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {

		std::cout << "SDL_mixer couldn't be initialized: " << Mix_GetError() << std::endl; 
		
		return; 

	}

	this->width = width;
	this->height = height;
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	if (window == NULL) {

		std::cout << "SDL_Window couldn't be initialized: " << SDL_GetError() << std::endl; 

		return; 

	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {

		std::cout << "SDL_Renderer couldn't be initialized: " << SDL_GetError() << std::endl;

		return;

	}

	for (int i = 0; i < 8; i++) {
		pieces.push_back(new Pieces("Pawn", "White", std::tuple<int, int>(i, 6), this, 1));
		pieces.push_back(new Pieces("Pawn", "Black", std::tuple<int, int>(i, 1), this, 1));
	}
	for (int i = 0; i < 2; i++) {

		pieces.push_back(new Pieces("Rook", "White", std::tuple<int, int>(i * 7, 7), this, 5));
		pieces.push_back(new Pieces("Rook", "Black", std::tuple<int, int>(i * 7, 0), this, 5));
	}

	for (int i = 0; i < 2; i++) {

		pieces.push_back(new Pieces("Knight", "White", std::tuple<int, int>(i * 5 + 1, 7), this, 3));
		pieces.push_back(new Pieces("Knight", "Black", std::tuple<int, int>(i * 5 + 1, 0), this, 3));

	}

	for (int i = 0; i < 2; i++) {

		pieces.push_back(new Pieces("Bishop", "White", std::tuple<int, int>(i * 3 + 2, 7), this, 3));
		pieces.push_back(new Pieces("Bishop", "Black", std::tuple<int, int>(i * 3 + 2, 0), this, 3));

	}

	pieces.push_back(new Pieces("Queen", "White", std::tuple<int, int>(3, 7), this, 9));
	pieces.push_back(new Pieces("Queen", "Black", std::tuple<int, int>(3, 0), this, 9));

	pieces.push_back(new Pieces("King", "White", std::tuple<int, int>(4, 7), this, 0));
	pieces.push_back(new Pieces("King", "Black", std::tuple<int, int>(4, 0), this, 0));

	//std::cout << pieces[0]->check(this).size() << std::endl; 

}

void Engine::draw() {

	for (int i = 0; i < this->get_pieces().size(); i++) {

		SDL_RenderCopy(this->get_renderer(), this->get_pieces()[i]->texture, &this->get_pieces()[i]->srcrect, &this->get_pieces()[i]->dstrect);

	}

}

void Engine::update(Events* events, Board* board, Engine* engine, Pieces* pieces) {
	for (int l = 0; l < engine->get_pieces().size(); l++) {
		if (engine->get_pieces()[l]->selected) {

			if (engine->get_pieces()[l]->name == "Pawn") {

				int abs_dist_x = std::abs(std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos));
				int dist_x = std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos);
				int abs_dist_y = std::abs(std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos));
				int dist_y = std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos);

				std::tuple<int, int> new_pos1;
				std::tuple<int, int> new_pos2;



				if (std::get<0>(events->get_mouse()) == std::get<0>(engine->get_pieces()[l]->pos)) {


					if (abs_dist_y != 0) {
						if (abs_dist_y > 2) {

							engine->get_pieces()[l]->selected = false;
							return;

						}

						if (abs_dist_y == 1 || abs_dist_y == 2) {

							if (dist_y < 0) { // Pawn going up

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < std::abs(std::get<1>(engine->get_pieces()[l]->pos) - std::get<1>(events->get_mouse())); j++) {

										std::tuple<int, int> check_pos;

										check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos), std::get<1>(engine->get_pieces()[l]->pos) - j - 1);
										//std::cout << "(" << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << ")" << std::endl;
										if (check_pos == engine->get_pieces()[i]->return_pos()) {

											//std::cout << "Collision" << std::endl;
											engine->get_pieces()[l]->selected = false;
											return;

										}

									}

								}
							}

							else if (dist_y > 0) { // Pawn going down

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < std::abs(std::get<1>(engine->get_pieces()[l]->pos) - std::get<1>(events->get_mouse())); j++) {

										std::tuple<int, int> check_pos;

										check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos), std::get<1>(engine->get_pieces()[l]->pos) + j + 1);
										//std::cout << "(" << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << ")" << std::endl;
										if (check_pos == engine->get_pieces()[i]->return_pos()) {

											//std::cout << "Collision" << std::endl;
											engine->get_pieces()[l]->selected = false;
											return;

										}

									}

								}

							}

						}

						if (engine->get_pieces()[l]->first_move && abs_dist_y <= 2) {

							if (abs_dist_y == 2) {

								engine->get_pieces()[l]->passant = true;
								engine->get_pieces()[l]->passant_turn = board->turn + 1;
							}
							engine->get_pieces()[l]->first_move = false; 
							this->move(events, board, engine, l, pieces);

							return;

						}

						else if (!engine->get_pieces()[l]->first_move && abs_dist_y == 1) {

							if (engine->get_pieces()[l]->color == "White" && dist_y < 0 || engine->get_pieces()[l]->color == "Black" && dist_y > 0) {

								this->move(events, board, engine, l, pieces);

								return;
							}
							else {

								engine->get_pieces()[l]->selected = false;
								return;

							}


						}
					}
				}

				// Capturing


				if (abs_dist_x == 1 && abs_dist_y == 1) { // En Passant

					std::tuple<int, int> test_tuple1 = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + 1, std::get<1>(engine->get_pieces()[l]->pos));
					std::tuple<int, int> test_tuple2 = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - 1, std::get<1>(engine->get_pieces()[l]->pos));

					for (int i = 0; i < engine->get_pieces().size(); i++) {
						if (test_tuple1 == engine->get_pieces()[i]->pos || test_tuple2 == engine->get_pieces()[i]->pos) {

							if (engine->get_pieces()[i]->passant && board->turn == engine->get_pieces()[i]->passant_turn) {
								if (std::get<0>(events->get_mouse()) == std::get<0>(engine->get_pieces()[i]->pos)) {
									if (engine->get_pieces()[l]->first_move)
										engine->get_pieces()[l]->first_move = false;
									this->capture(events, board, engine, l, i, pieces);

									return;
								}
							}

						}
					}
				}
				if (abs_dist_y == 1 && abs_dist_x != 0) {

					if (engine->get_pieces()[l]->color == "White" && dist_y < 0 || engine->get_pieces()[l]->color == "Black" && dist_y > 0) {

						if (abs_dist_x == 1) {

							std::tuple<int, int> coords = pieces->get_coords(events->get_mouse());
							SDL_Rect rect = { std::get<0>(coords), std::get<1>(coords), engine->get_pieces()[l]->w, engine->get_pieces()[l]->h };
							for (int i = 0; i < engine->get_pieces().size(); i++) {

								if (rect.x == engine->get_pieces()[i]->dstrect.x && rect.y == engine->get_pieces()[i]->dstrect.y && engine->get_pieces()[l]->color != engine->get_pieces()[i]->color) {

									if (engine->get_pieces()[l]->first_move)
										engine->get_pieces()[l]->first_move = false;
									this->capture(events, board, engine, l, i, pieces);

									return;

								}

								else if (rect.x == engine->get_pieces()[i]->dstrect.x && rect.y == engine->get_pieces()[i]->dstrect.y && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

									engine->get_pieces()[l]->selected = false;
									return;

								}


							}
							return;

						}

					}

				}

			}

			else if (engine->get_pieces()[l]->name == "Rook") {

				int abs_dist_x = std::abs(std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos));
				int abs_dist_y = std::abs(std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos));
				int dist_x = std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos);
				int dist_y = std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos);

				if (engine->get_pieces()[l]->selected) {
					if (dist_x == 0 && dist_y != 0 || dist_x != 0 && dist_y == 0) {
						// Check collision 

						if (abs_dist_y == 1 || abs_dist_x == 1) {

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

									engine->get_pieces()[l]->selected = false;
									break;

								}
							}

						}

						if (dist_y < 0) { // Going up

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_y; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos), std::get<1>(engine->get_pieces()[l]->pos) - j);

									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break; // Can comment this out
									}

								}
							}

						}

						else if (dist_y > 0) { // Going down

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_y; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos), std::get<1>(engine->get_pieces()[l]->pos) + j);
									std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break;


									}

								}
							}

						}

						else if (dist_x < 0) { // Going left
							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_x; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - j, std::get<1>(engine->get_pieces()[l]->pos));
									std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break;


									}

								}
							}

						}

						else if (dist_x > 0) { // Going right
							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_x; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos));
									std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;


									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break;


									}

								}
							}

						}

						// Capturing
						for (int i = 0; i < engine->get_pieces().size(); i++) {
							if (engine->get_pieces()[l]->selected) {
								if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color != engine->get_pieces()[i]->color) {

									if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(0, 7))) {

										engine->get_pieces()[l]->white_rook_a_first_move = false;

									}

									else if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(7, 7))) {

										engine->get_pieces()[l]->white_rook_h_first_move = false;

									}
									else if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(0, 0))) {

										engine->get_pieces()[l]->black_rook_a_first_move = false;

									}

									else if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(7, 0))) {

										engine->get_pieces()[l]->black_rook_h_first_move = false;

									}
									this->capture(events, board, engine, l, i, pieces);
									break;

								}

								else if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

									engine->get_pieces()[l]->selected = false;
									return;

								}
							}
						}


						// Moving 
						for (int i = 0; i < engine->get_pieces().size(); i++) {

							if (engine->get_pieces()[l]->selected) {
								if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(0, 7))) {

									engine->get_pieces()[l]->white_rook_a_first_move = false;

								}

								else if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(7, 7))) {

									engine->get_pieces()[l]->white_rook_h_first_move = false;

								}
								else if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(0, 0))) {

									engine->get_pieces()[l]->black_rook_a_first_move = false;

								}

								else if (engine->get_pieces()[l]->pos == (std::tuple<int, int>(7, 0))) {

									engine->get_pieces()[l]->black_rook_h_first_move = false;

								}
								this->move(events, board, engine, l, pieces);
								break;
							}
						}
					}
				}


			}

			else if (engine->get_pieces()[l]->name == "Bishop") {

				int abs_dist_x = std::abs(std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos));
				int abs_dist_y = std::abs(std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos));
				int dist_x = std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos);
				int dist_y = std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos);

				if (engine->get_pieces()[l]->selected) {

					if (abs_dist_x == 0 && abs_dist_y == 0) {
						return;
					}
					else if (abs_dist_x == abs_dist_y) {
						// Check collision

						if (abs_dist_x == 1 && abs_dist_y == 1) {

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

									engine->get_pieces()[l]->selected = false;
									break;

								}
							}

						}

						if (dist_x > 0) { // Right

							if (dist_y < 0) { // Up

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos) - j);
										//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

							else if (dist_y > 0) { // Down

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos) + j);
										//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

						}

						else if (dist_x < 0) { // Left

							if (dist_y > 0) { // Down 

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - j, std::get<1>(engine->get_pieces()[l]->pos) + j);
										//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

							else if (dist_y < 0) { // Up

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - j, std::get<1>(engine->get_pieces()[l]->pos) - j);
										//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

						}

						// Capturing
						for (int i = 0; i < engine->get_pieces().size(); i++) {
							if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color != engine->get_pieces()[i]->color) {

								this->capture(events, board, engine, l, i, pieces);
								break;

							}
							else if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

								engine->get_pieces()[l]->selected = false;
								return;

							}
						}

						// Moving
						for (int i = 0; i < engine->get_pieces().size(); i++) {

							if (engine->get_pieces()[l]->selected) {
								this->move(events, board, engine, l, pieces);
								break;
							}
						}

					}
				}

			}

			else if (engine->get_pieces()[l]->name == "Knight") {

				int dist_x = std::abs(std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos));
				int dist_y = std::abs(std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos));

				std::tuple<int, int> coords = pieces->get_coords(events->get_mouse());
				SDL_Rect rect = { std::get<0>(coords), std::get<1>(coords), engine->get_pieces()[l]->w, engine->get_pieces()[l]->h };

				if (engine->get_pieces()[l]->selected) {
					if (dist_x == 0 && dist_y == 0) {

						return;
					}
					else if (dist_x == 1 && dist_y == 2 || dist_x == 2 && dist_y == 1) {

						for (int i = 0; i < engine->get_pieces().size(); i++) {

							// Capturing

							if (rect.x == engine->get_pieces()[i]->dstrect.x && rect.y == engine->get_pieces()[i]->dstrect.y && engine->get_pieces()[l]->color != engine->get_pieces()[i]->color) {
								this->capture(events, board, engine, l, i, pieces);
								return;

							}

							else if (rect.x == engine->get_pieces()[i]->dstrect.x && rect.y == engine->get_pieces()[i]->dstrect.y && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

								engine->get_pieces()[l]->selected = false;
								return;

							}

						}

						if (engine->get_pieces()[l]->selected) {
							this->move(events, board, engine, l, pieces);
							return;
						}


					}

					else {

						engine->get_pieces()[l]->selected = false;

						return;

					}
				}
			}

			else if (engine->get_pieces()[l]->name == "Queen") {

				int abs_dist_x = std::abs(std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos));
				int abs_dist_y = std::abs(std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos));
				int dist_x = std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos);
				int dist_y = std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos);

				if (engine->get_pieces()[l]->selected) {

					if (abs_dist_x == 0 && abs_dist_y == 0)
						return;
					if (dist_x == 0 && dist_y != 0 || dist_x != 0 && dist_y == 0) {
						// Check collision 

						if (abs_dist_y == 1 || abs_dist_x == 1) {

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

									engine->get_pieces()[l]->selected = false;
									break;

								}
							}

						}

						if (dist_y < 0) { // Going up

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_y; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos), std::get<1>(engine->get_pieces()[l]->pos) - j);

									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break; // Can comment this out
									}

								}
							}

						}

						else if (dist_y > 0) { // Going down

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_y; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos), std::get<1>(engine->get_pieces()[l]->pos) + j);
									//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break;


									}

								}
							}

						}

						else if (dist_x < 0) { // Going left
							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_x; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - j, std::get<1>(engine->get_pieces()[l]->pos));
									//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break;


									}

								}
							}

						}

						else if (dist_x > 0) { // Going right
							for (int i = 0; i < engine->get_pieces().size(); i++) {
								for (int j = 0; j < abs_dist_x; j++) {

									std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos));
									//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;


									if (check_pos == engine->get_pieces()[l]->pos) {

										//

									}

									else if (check_pos == engine->get_pieces()[i]->pos) {

										//std::cout << "not valid" << std::endl;
										engine->get_pieces()[l]->selected = false;
										break;


									}

								}
							}

						}
					}
					else if (abs_dist_x == abs_dist_y) {

						// Check collision

						if (abs_dist_x == 1 && abs_dist_y == 1) {

							for (int i = 0; i < engine->get_pieces().size(); i++) {
								if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

									engine->get_pieces()[l]->selected = false;
									break;

								}
							}

						}

						if (dist_x > 0) { // Right

							if (dist_y < 0) { // Up

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos) - j);
										//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

							else if (dist_y > 0) { // Down

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos) + j);

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

						}

						else if (dist_x < 0) { // Left

							if (dist_y > 0) { // Down 

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - j, std::get<1>(engine->get_pieces()[l]->pos) + j);

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

							else if (dist_y < 0) { // Up

								for (int i = 0; i < engine->get_pieces().size(); i++) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) - j, std::get<1>(engine->get_pieces()[l]->pos) - j);

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}

										else if (check_pos == engine->get_pieces()[i]->pos) {

											//std::cout << "not valid" << std::endl;
											engine->get_pieces()[l]->selected = false;
											break;


										}

									}

								}

							}

						}
					}

					else if (dist_x != 0 && dist_y != 0 || dist_x != 0 && dist_y != 0 && abs_dist_x != abs_dist_y) {

						engine->get_pieces()[l]->selected = false;
						return;

					}

					// Capturing
					for (int i = 0; i < engine->get_pieces().size(); i++) {
						if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color != engine->get_pieces()[i]->color) {

							this->capture(events, board, engine, l, i, pieces);
							return;

						}
						else if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

							engine->get_pieces()[l]->selected = false;
							return;

						}
					}

					// Moving

					for (int i = 0; i < engine->get_pieces().size(); i++) {

						if (engine->get_pieces()[l]->selected) {
							this->move(events, board, engine, l, pieces);
							break;
						}
					}

				}

			}

			else if (engine->get_pieces()[l]->name == "King") {

				int abs_dist_x = std::abs(std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos));
				int abs_dist_y = std::abs(std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos));
				int dist_x = std::get<0>(events->get_mouse()) - std::get<0>(engine->get_pieces()[l]->pos);
				int dist_y = std::get<1>(events->get_mouse()) - std::get<1>(engine->get_pieces()[l]->pos);

				if (engine->get_pieces()[l]->selected) {

					if (abs_dist_x == 1 && abs_dist_y <= 1 || abs_dist_y == 1 && abs_dist_x <= 1) {

						// Collision

						for (int i = 0; i < engine->get_pieces().size(); i++) {

							if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

								engine->get_pieces()[l]->selected = false;

								break;

							}

						}

						if (this->check(engine, events->get_mouse(), engine->get_pieces()[l]->color, l)) {

							std::cout << "COVERED" << std::endl;
							engine->get_pieces()[l]->selected = false;
							return;

						}

						// Capture

						for (int i = 0; i < engine->get_pieces().size(); i++) {


							if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color != engine->get_pieces()[i]->color) {

								if (engine->get_pieces()[l]->pos == std::tuple<int, int>(4, 7)) {

									engine->get_pieces()[l]->white_king_first_move = false;

								}
								else if (engine->get_pieces()[l]->pos == std::tuple<int, int>(0, 7)) {

									engine->get_pieces()[l]->black_king_first_move = false;

								}
								this->capture(events, board, engine, l, i, pieces);
								break;

							}

							else if (events->get_mouse() == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color) {

								engine->get_pieces()[l]->selected = false;
								break;

							}

						}


						// Movement

						for (int i = 0; i < engine->get_pieces().size(); i++) {

							if (engine->get_pieces()[l]->selected) {

								if (engine->get_pieces()[l]->pos == std::tuple<int, int>(4, 7)) {

									engine->get_pieces()[l]->white_king_first_move = false;

								}
								else if (engine->get_pieces()[l]->pos == std::tuple<int, int>(0, 7)) {

									engine->get_pieces()[l]->black_king_first_move = false;

								}

								this->move(events, board, engine, l, pieces);
								break;
							}
						}



						
					}
					// Castling
					else if (abs_dist_x == 2 && abs_dist_y == 0) {

						int index = 0;
						for (int i = 0; i < engine->get_pieces().size(); i++) {

							if (std::tuple<int, int>(6, 7) == events->get_mouse() && std::tuple<int, int>(7, 7) == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color && engine->get_pieces()[i]->name == "Rook") {

								index = i;
								break;

							}

							else if (std::tuple<int, int>(6, 0) == events->get_mouse() && std::tuple<int, int>(7, 0) == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color && engine->get_pieces()[i]->name == "Rook") {

								index = i;
								break;

							}

							else if (std::tuple<int, int>(2, 0) == events->get_mouse() && std::tuple<int, int>(0, 0) == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color && engine->get_pieces()[i]->name == "Rook") {

								index = i;
								break;

							}

							else if (std::tuple<int, int>(2, 7) == events->get_mouse() && std::tuple<int, int>(0, 7) == engine->get_pieces()[i]->pos && engine->get_pieces()[l]->color == engine->get_pieces()[i]->color && engine->get_pieces()[i]->name == "Rook") {
								index = i;
								break;

							}

						}

						for (int i = 0; i < engine->get_pieces().size(); i++) {

							if (engine->get_pieces()[l]->selected) {

								if (events->get_mouse() == (std::tuple<int, int>(6, 7)) && engine->get_pieces()[l]->white_king_first_move && engine->get_pieces()[index]->white_rook_h_first_move) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos));

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}
										else if (check_pos == engine->get_pieces()[i]->pos) {

											engine->get_pieces()[l]->selected = false;
											return;

										}

										else {
											//
											this->move(events, board, engine, l, pieces);

											engine->get_pieces()[l]->white_king_first_move = false;
											engine->get_pieces()[l]->white_rook_h_first_move = false;
											engine->get_pieces()[index]->pos = std::tuple<int, int>(5, 7);
											engine->get_pieces()[index]->coords = pieces->get_coords(engine->get_pieces()[index]->pos);
											engine->get_pieces()[index]->dstrect = { std::get<0>(engine->get_pieces()[index]->coords), std::get<1>(engine->get_pieces()[index]->coords), engine->get_pieces()[l]->w, engine->get_pieces()[l]->h };
											engine->get_pieces()[index]->white_rook_h_first_move = false;

											return;
										}

									}

								}

								else if (events->get_mouse() == (std::tuple<int, int>(2, 7)) && engine->get_pieces()[l]->white_king_first_move && engine->get_pieces()[index]->white_rook_a_first_move) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos));

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}
										else if (check_pos == engine->get_pieces()[i]->pos) {

											engine->get_pieces()[l]->selected = false;
											break;

										}

										else {
											//std::cout << index << std::endl;
											this->move(events, board, engine, l, pieces);
											engine->get_pieces()[l]->white_king_first_move = false;
											engine->get_pieces()[l]->white_rook_a_first_move = false;
											engine->get_pieces()[index]->pos = std::tuple<int, int>(3, 7);
											engine->get_pieces()[index]->coords = pieces->get_coords(engine->get_pieces()[index]->pos);
											engine->get_pieces()[index]->dstrect = { std::get<0>(engine->get_pieces()[index]->coords), std::get<1>(engine->get_pieces()[index]->coords), engine->get_pieces()[l]->w, engine->get_pieces()[l]->h };
											engine->get_pieces()[index]->white_rook_a_first_move = false;

											break;
										}

									}

								}

								else if (events->get_mouse() == (std::tuple<int, int>(2, 0)) && engine->get_pieces()[l]->black_king_first_move && engine->get_pieces()[index]->black_rook_a_first_move) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos));

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}
										else if (check_pos == engine->get_pieces()[i]->pos) {

											engine->get_pieces()[l]->selected = false;
											break;

										}

										else {
											//std::cout << index << std::endl;
											this->move(events, board, engine, l, pieces);
											engine->get_pieces()[l]->black_king_first_move = false;
											engine->get_pieces()[l]->black_rook_a_first_move = false;
											engine->get_pieces()[index]->pos = std::tuple<int, int>(3, 0);
											engine->get_pieces()[index]->coords = pieces->get_coords(engine->get_pieces()[index]->pos);
											engine->get_pieces()[index]->dstrect = { std::get<0>(engine->get_pieces()[index]->coords), std::get<1>(engine->get_pieces()[index]->coords), engine->get_pieces()[l]->w, engine->get_pieces()[l]->h };
											engine->get_pieces()[index]->black_rook_a_first_move = false;
											break;
										}

									}

								}

								else if (events->get_mouse() == (std::tuple<int, int>(6, 0)) && engine->get_pieces()[l]->black_king_first_move && engine->get_pieces()[index]->black_rook_h_first_move) {

									for (int j = 0; j < abs_dist_x; j++) {

										std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[l]->pos) + j, std::get<1>(engine->get_pieces()[l]->pos));

										if (check_pos == engine->get_pieces()[l]->pos) {

											//

										}
										else if (check_pos == engine->get_pieces()[i]->pos) {

											engine->get_pieces()[l]->selected = false;
											break;

										}

										else {
											//std::cout << index << std::endl;
											this->move(events, board, engine, l, pieces);
											engine->get_pieces()[l]->black_king_first_move = false;
											engine->get_pieces()[l]->black_rook_a_first_move = false;
											engine->get_pieces()[index]->pos = std::tuple<int, int>(5, 0);
											engine->get_pieces()[index]->coords = pieces->get_coords(engine->get_pieces()[index]->pos);
											engine->get_pieces()[index]->dstrect = { std::get<0>(engine->get_pieces()[index]->coords), std::get<1>(engine->get_pieces()[index]->coords), engine->get_pieces()[l]->w, engine->get_pieces()[l]->h };
											engine->get_pieces()[index]->black_rook_h_first_move = false;
											break;
										}

									}

								}
							}

						}

					}

				}

			}

		}

	}

}

void Engine::move(Events* events, Board* board, Engine* engine, int index, Pieces* pieces) {

	std::tuple<int, int> temp = engine->get_pieces()[index]->pos;
	engine->get_pieces()[index]->pos = events->get_mouse();

	for (int i = 0; i < engine->get_pieces().size(); i++) {

		if (engine->get_pieces()[i]->name == "King" && !engine->get_pieces()[i]->in_check) {

			if (this->check(engine, engine->get_pieces()[i]->pos, engine->get_pieces()[i]->color, i)) {

				engine->get_pieces()[i]->in_check = true;

			}

		}

		else if (engine->get_pieces()[i]->name == "King" && engine->get_pieces()[i]->in_check) {

			if (this->check(engine, engine->get_pieces()[i]->pos, engine->get_pieces()[i]->color, i)) {

				engine->get_pieces()[index]->selected = false;
				engine->get_pieces()[index]->pos = temp;
				return;

			}

		}

	}

	for (int i = 0; i < engine->get_pieces().size(); i++) {

		if (engine->get_pieces()[i]->name == "King" && engine->get_pieces()[i]->in_check) {

			if (!this->check(engine, engine->get_pieces()[i]->pos, engine->get_pieces()[i]->color, i)) {

				engine->get_pieces()[i]->in_check = false;

			}

		}

	}

	board->turn += 1;

	engine->get_pieces()[index]->coords = pieces->get_coords(engine->get_pieces()[index]->pos);
	engine->get_pieces()[index]->dstrect = { std::get<0>(engine->get_pieces()[index]->coords), std::get<1>(engine->get_pieces()[index]->coords), engine->get_pieces()[index]->w, engine->get_pieces()[index]->h };
	engine->get_pieces()[index]->selected = false;

}

void Engine::capture(Events* events, Board* board, Engine* engine, int move_index, int capture_index, Pieces* pieces) {

	std::tuple<int, int> temp = engine->get_pieces()[move_index]->pos;
	engine->get_pieces()[move_index]->pos = events->get_mouse();

	for (int i = 0; i < engine->get_pieces().size(); i++) {

		if (engine->get_pieces()[i]->name == "King" && !engine->get_pieces()[i]->in_check) {

			if (this->check(engine, engine->get_pieces()[i]->pos, engine->get_pieces()[i]->color, i)) {

				engine->get_pieces()[i]->in_check = true;

			}

		}

		else if (engine->get_pieces()[i]->name == "King" && engine->get_pieces()[i]->in_check) {

			if (this->check(engine, engine->get_pieces()[i]->pos, engine->get_pieces()[i]->color, i)) {

				engine->get_pieces()[move_index]->selected = false;
				engine->get_pieces()[move_index]->pos = temp;
				return;

			}

		}

	}
	board->turn += 1;
	engine->get_pieces()[move_index]->coords = pieces->get_coords(engine->get_pieces()[move_index]->pos);
	engine->get_pieces()[move_index]->dstrect = { std::get<0>(engine->get_pieces()[move_index]->coords), std::get<1>(engine->get_pieces()[move_index]->coords), engine->get_pieces()[move_index]->w, engine->get_pieces()[move_index]->h };
	engine->get_pieces()[move_index]->selected = false;

	engine->destroy_piece(capture_index);

}

bool Engine::check(Engine* engine, std::tuple<int, int> pos, std::string color, int index) {

	std::vector<std::tuple<int, int, std::string, std::string>> attacked_squares;

	for (int i = 0; i < engine->get_pieces().size(); i++) {

		if (engine->get_pieces()[i]->name == "Pawn") {

			if (engine->get_pieces()[i]->color == "White") {

				std::tuple<int, int, std::string, std::string> test1 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
				std::tuple<int, int, std::string, std::string> test2 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
				if (std::get<0>(test1) >= 0 && std::get<0>(test1) <= 7 && std::get<1>(test1) > 0) {
					attacked_squares.push_back(test1);

				}
				if (std::get<0>(test2) >= 0 && std::get<0>(test2) <= 7 && std::get<1>(test2) > 0) {
					attacked_squares.push_back(test2);
				}

			}

			else if (engine->get_pieces()[i]->color == "Black") {

				std::tuple<int, int, std::string, std::string> test1 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
				std::tuple<int, int, std::string, std::string> test2 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);

				if (std::get<0>(test1) >= 0 && std::get<0>(test1) <= 7 && std::get<1>(test1) > 0) {
					attacked_squares.push_back(test1);
				}
				if (std::get<0>(test2) >= 0 && std::get<0>(test2) <= 7 && std::get<1>(test2) > 0) {
					attacked_squares.push_back(test2);
				}

			}

		}

		else if (engine->get_pieces()[i]->name == "Knight") {

			std::tuple<int, int, std::string, std::string> test1 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 2, std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test2 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 2, std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test3 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 2, std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test4 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 2, std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test5 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos) + 2, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test6 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos) - 2, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test7 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos) + 2, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
			std::tuple<int, int, std::string, std::string> test8 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos) - 2, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);


			if (std::get<0>(test1) >= 0 && std::get<0>(test1) <= 7 && std::get<1>(test1) >= 0 && std::get<1>(test1) <= 7) {
				attacked_squares.push_back(test1);
			}
			if (std::get<0>(test2) >= 0 && std::get<0>(test2) <= 7 && std::get<1>(test2) >= 0 && std::get<1>(test2) <= 7) {
				attacked_squares.push_back(test2);
			}
			if (std::get<0>(test3) >= 0 && std::get<0>(test3) <= 7 && std::get<1>(test3) >= 0 && std::get<1>(test3) <= 7) {
				attacked_squares.push_back(test3);
			}
			if (std::get<0>(test4) >= 0 && std::get<0>(test4) <= 7 && std::get<1>(test4) >= 0 && std::get<1>(test4) <= 7) {
				attacked_squares.push_back(test4);
			}
			if (std::get<0>(test5) >= 0 && std::get<0>(test5) <= 7 && std::get<1>(test5) >= 0 && std::get<1>(test5) <= 7) {
				attacked_squares.push_back(test5);
			}
			if (std::get<0>(test6) >= 0 && std::get<0>(test6) <= 7 && std::get<1>(test6) >= 0 && std::get<1>(test6) <= 7) {
				attacked_squares.push_back(test6);
			}
			if (std::get<0>(test7) >= 0 && std::get<0>(test7) <= 7 && std::get<1>(test7) >= 0 && std::get<1>(test7) <= 7) {
				attacked_squares.push_back(test7);
			}
			if (std::get<0>(test8) >= 0 && std::get<0>(test8) <= 7 && std::get<1>(test8) >= 0 && std::get<1>(test8) <= 7) {
				attacked_squares.push_back(test8);
			}


		}

		else if (engine->get_pieces()[i]->name == "Rook") {

			std::tuple<int, int, std::string, std::string> check_pos;
			std::tuple<int, int> test_pos;

			for (int k = 0; k < 8; k++) { // Right
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + k, std::get<1>(engine->get_pieces()[i]->pos), engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						goto right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}
		right_stop:
			std::cout << "";
			for (int k = 0; k < 8; k++) { // Left
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - k, std::get<1>(engine->get_pieces()[i]->pos), engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}
						goto left_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}


				}

			}
		left_stop:
			std::cout << "";
			for (int k = 0; k < 8; k++) { // Up
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos), std::get<1>(engine->get_pieces()[i]->pos) - k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					if (std::get<0>(check_pos) >= 0 && std::get<0>(check_pos) <= 7 && std::get<1>(check_pos) >= 0 && std::get<1>(check_pos) <= 7) {
						//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

							//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;

							attacked_squares.push_back(check_pos);

							int counter = 0;
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h]) {

									counter += 1;

								}

							}
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h] && counter > 1) {

									attacked_squares.erase(attacked_squares.begin() + h);

								}

							}

							goto up_stop;

						}
						else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

							//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
							attacked_squares.push_back(check_pos);
							int counter = 0;
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h]) {

									counter += 1;

								}

							}
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h] && counter > 1) {

									attacked_squares.erase(attacked_squares.begin() + h);

								}

							}

						}
					}
				}

			}
		up_stop:
			std::cout << "";
			for (int k = 0; k < 8; k++) { // Down
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos), std::get<1>(engine->get_pieces()[i]->pos) + k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}
						goto down_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}


				}

			}
		down_stop:
			std::cout << "";


		}

		else if (engine->get_pieces()[i]->name == "Bishop") {

			std::tuple<int, int, std::string, std::string> check_pos;
			std::tuple<int, int> test_pos;

			for (int k = 0; k < 8; k++) { // Up Right
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + k, std::get<1>(engine->get_pieces()[i]->pos) - k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						goto up_right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}

		up_right_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { // Up Left
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - k, std::get<1>(engine->get_pieces()[i]->pos) - k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						goto up_left_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}

		up_left_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { // Down right
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + k, std::get<1>(engine->get_pieces()[i]->pos) + k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						goto down_right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}
		down_right_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { //Down left
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - k, std::get<1>(engine->get_pieces()[i]->pos) + k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						goto down_left_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}
		down_left_stop:
			std::cout << "";
		}

		else if (engine->get_pieces()[i]->name == "Queen") {

			std::tuple<int, int, std::string, std::string> check_pos;
			std::tuple<int, int> test_pos;

			for (int k = 0; k < 8; k++) { // Right
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + k, std::get<1>(engine->get_pieces()[i]->pos), engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					//std::cout << std::get<0>(engine->get_pieces()[i]->pos) << ", " << std::get<1>(engine->get_pieces()[i]->pos) << std::endl; 
					//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						goto queen_right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}
		queen_right_stop:
			std::cout << "";
			for (int k = 0; k < 8; k++) { // Left
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - k, std::get<1>(engine->get_pieces()[i]->pos), engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}
						goto queen_left_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}


				}

			}
		queen_left_stop:
			std::cout << "";
			for (int k = 0; k < 8; k++) { // Up
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos), std::get<1>(engine->get_pieces()[i]->pos) - k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));
					if (std::get<0>(check_pos) >= 0 && std::get<0>(check_pos) <= 7 && std::get<1>(check_pos) >= 0 && std::get<1>(check_pos) <= 7) {

						if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

							attacked_squares.push_back(check_pos);

							int counter = 0;
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h]) {

									counter += 1;

								}

							}
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h] && counter > 1) {

									attacked_squares.erase(attacked_squares.begin() + h);

								}

							}

							goto queen_up_stop;

						}
						else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

							attacked_squares.push_back(check_pos);
							int counter = 0;
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h]) {

									counter += 1;

								}

							}
							for (int h = 0; h < attacked_squares.size(); h++) {

								if (check_pos == attacked_squares[h] && counter > 1) {

									attacked_squares.erase(attacked_squares.begin() + h);

								}

							}

						}
					}
				}

			}
		queen_up_stop:
			std::cout << "";
			for (int k = 0; k < 8; k++) { // Down
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos), std::get<1>(engine->get_pieces()[i]->pos) + k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}
						goto queen_down_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}


				}

			}
		queen_down_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { // Up Right
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + k, std::get<1>(engine->get_pieces()[i]->pos) - k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						goto queen_up_right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}

		queen_up_right_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { // Up Left
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - k, std::get<1>(engine->get_pieces()[i]->pos) - k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						goto queen_up_left_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}

		queen_up_left_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { // Down right
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + k, std::get<1>(engine->get_pieces()[i]->pos) + k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						goto queen_down_right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}
		queen_down_right_stop:
			std::cout << "";

			for (int k = 0; k < 8; k++) { //Down left
				for (int j = 0; j < engine->get_pieces().size(); j++) {

					check_pos = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - k, std::get<1>(engine->get_pieces()[i]->pos) + k, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name);
					test_pos = std::make_tuple(std::get<0>(check_pos), std::get<1>(check_pos));

					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						goto queen_down_left_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h] && counter > 1) {

								attacked_squares.erase(attacked_squares.begin() + h);

							}

						}

					}

				}

			}
		queen_down_left_stop:
			std::cout << "";

		}

		else if (engine->get_pieces()[i]->name == "King") {

			std::tuple<int, int, std::string, std::string> test1, test2, test3, test4, test5, test6, test7, test8;

			test1 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos), engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Right
			test2 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos), engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Left
			test3 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos), std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Down
			test4 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos), std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Up
			test5 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Up right
			test6 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos) - 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Up left 
			test7 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) + 1, std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Down right
			test8 = std::make_tuple(std::get<0>(engine->get_pieces()[i]->pos) - 1, std::get<1>(engine->get_pieces()[i]->pos) + 1, engine->get_pieces()[i]->color, engine->get_pieces()[i]->name); // Down left

			if (std::get<0>(test1) >= 0 && std::get<0>(test1) <= 7 && std::get<1>(test1) >= 0 && std::get<1>(test1) <= 7) {
				attacked_squares.push_back(test1);
			}
			if (std::get<0>(test2) >= 0 && std::get<0>(test2) <= 7 && std::get<1>(test2) >= 0 && std::get<1>(test2) <= 7) {
				attacked_squares.push_back(test2);
			}
			if (std::get<0>(test3) >= 0 && std::get<0>(test3) <= 7 && std::get<1>(test3) >= 0 && std::get<1>(test3) <= 7) {
				attacked_squares.push_back(test3);
			}
			if (std::get<0>(test4) >= 0 && std::get<0>(test4) <= 7 && std::get<1>(test4) >= 0 && std::get<1>(test4) <= 7) {
				attacked_squares.push_back(test4);
			}
			if (std::get<0>(test5) >= 0 && std::get<0>(test5) <= 7 && std::get<1>(test5) >= 0 && std::get<1>(test5) <= 7) {
				attacked_squares.push_back(test5);
			}
			if (std::get<0>(test6) >= 0 && std::get<0>(test6) <= 7 && std::get<1>(test6) >= 0 && std::get<1>(test6) <= 7) {
				attacked_squares.push_back(test6);
			}
			if (std::get<0>(test7) >= 0 && std::get<0>(test7) <= 7 && std::get<1>(test7) >= 0 && std::get<1>(test7) <= 7) {
				attacked_squares.push_back(test7);
			}
			if (std::get<0>(test8) >= 0 && std::get<0>(test8) <= 7 && std::get<1>(test8) >= 0 && std::get<1>(test8) <= 7) {
				attacked_squares.push_back(test8);
			}


		}

	}


	for (int i = 0; i < attacked_squares.size(); i++) {

		std::tuple<int, int> attacked = std::make_tuple(std::get<0>(attacked_squares[i]), std::get<1>(attacked_squares[i]));
		std::string col = std::get<2>(attacked_squares[i]);
		std::string name = std::get<3>(attacked_squares[i]);

		if (std::get<0>(attacked) == std::get<0>(pos) && std::get<1>(attacked) == std::get<1>(pos) && col != color) {

			return true;

		}


	}

	return false;

}

std::vector<std::tuple<int, int>> Engine::find_legal_moves(Board* board, Events* events) {
	
	std::vector<std::tuple<int, int>> legal_moves;

	std::string color;

	if (board->turn % 2 == 1)
		color = "White";
	else {

		color = "Black";

	}

	for (int i = 0; i < this->get_pieces().size(); i++) {

		if (this->get_pieces()[i]->color == color) {

			if (this->get_pieces()[i]->name == "Pawn") {
				std::tuple<int, int> check_pos1;
				std::tuple<int, int> check_pos2;

				bool pawn_check1 = false;
				bool pawn_check2 = false;
				if (color == "White") {

					// Capturing 

					pawn_check1 = false;
					pawn_check2 = false;

					check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) - 1, std::get<1>(this->get_pieces()[i]->pos) - 1);
					check_pos2 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + 1, std::get<1>(this->get_pieces()[i]->pos) - 1);

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos1 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color != this->get_pieces()[j]->color) {

							pawn_check1 = true; 

						}

						if (check_pos2 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color != this->get_pieces()[j]->color) {

							pawn_check2 = true; 

						}

					}

					if (pawn_check1) 
						legal_moves.push_back(check_pos1);
					if (pawn_check2)
						legal_moves.push_back(check_pos2);

					// Moving 

					if (this->get_pieces()[i]->first_move) {
						check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos), std::get<1>(this->get_pieces()[i]->pos) - 1);
						check_pos2 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos), std::get<1>(this->get_pieces()[i]->pos) - 2);
						pawn_check1 = false;
						pawn_check2 = false;
						for (int j = 0; j < this->get_pieces().size(); j++) {
							if (check_pos1 == this->get_pieces()[j]->pos ) {

								pawn_check1 = true; 

							}

							else if (check_pos2 == this->get_pieces()[j]->pos) {

								pawn_check2 = true; 

							}

						}
						if (!pawn_check1) {

							legal_moves.push_back(check_pos1);

						}

						if (!pawn_check2 && !pawn_check1) {

							legal_moves.push_back(check_pos2);

						}

					}
					else {

						check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos), std::get<1>(this->get_pieces()[i]->pos) - 1);
						for (int j = 0; j < this->get_pieces().size(); j++) {

							if (check_pos1 == this->get_pieces()[j]->pos) {

								pawn_check1 = true; 
								break;

							}

						}

						if (!pawn_check1) {

							legal_moves.push_back(check_pos1);

						}


					}
				}

				else if (color == "Black") {

					// Capturing 

					pawn_check1 = false;
					pawn_check2 = false;

					check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) - 1, std::get<1>(this->get_pieces()[i]->pos) + 1);
					check_pos2 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + 1, std::get<1>(this->get_pieces()[i]->pos) + 1);

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos1 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color != this->get_pieces()[j]->color) {

							pawn_check1 = true;

						}

						if (check_pos2 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color != this->get_pieces()[j]->color) {

							pawn_check2 = true;

						}

					}

					if (pawn_check1)
						legal_moves.push_back(check_pos1);
					if (pawn_check2)
						legal_moves.push_back(check_pos2);

					// Moving

					if (this->get_pieces()[i]->first_move) {
						check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos), std::get<1>(this->get_pieces()[i]->pos) + 1);
						check_pos2 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos), std::get<1>(this->get_pieces()[i]->pos) + 2);
						pawn_check1 = false;
						pawn_check2 = false;
						for (int j = 0; j < this->get_pieces().size(); j++) {
							if (check_pos1 == this->get_pieces()[j]->pos) {

								pawn_check1 = true;

							}

							else if (check_pos2 == this->get_pieces()[j]->pos) {

								pawn_check2 = true;

							}

						}
						if (!pawn_check1) {

							legal_moves.push_back(check_pos1);

						}

						if (!pawn_check2 && !pawn_check1) {

							legal_moves.push_back(check_pos2);

						}

					}
					else {

						check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos), std::get<1>(this->get_pieces()[i]->pos) + 1);
						for (int j = 0; j < this->get_pieces().size(); j++) {

							if (check_pos1 != this->get_pieces()[j]->pos) {

								pawn_check1 = true;
								break;

							}

						}

						if (!pawn_check1) {

							legal_moves.push_back(check_pos1);

						}


					}
				}

			}

			else if (this->get_pieces()[i]->name == "Knight") {

				std::tuple<int, int> check_pos1, check_pos2, check_pos3, check_pos4, check_pos5, check_pos6, check_pos7, check_pos8;

				check_pos1 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + 2, std::get<1>(this->get_pieces()[i]->pos) + 1);
				check_pos2 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + 2, std::get<1>(this->get_pieces()[i]->pos) - 1);
				check_pos3 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) - 2, std::get<1>(this->get_pieces()[i]->pos) + 1);
				check_pos4 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) - 2, std::get<1>(this->get_pieces()[i]->pos) - 1);
				check_pos5 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + 1, std::get<1>(this->get_pieces()[i]->pos) + 2);
				check_pos6 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + 1, std::get<1>(this->get_pieces()[i]->pos) - 2);
				check_pos7 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) - 1, std::get<1>(this->get_pieces()[i]->pos) + 2);
				check_pos8 = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) - 1, std::get<1>(this->get_pieces()[i]->pos) - 2);

				// Moving

				if (std::get<0>(check_pos1) >= 0 && std::get<0>(check_pos1) <= 7 && std::get<1>(check_pos1) >= 0 && std::get<1>(check_pos1) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos1 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos1);

				}

				if (std::get<0>(check_pos2) >= 0 && std::get<0>(check_pos2) <= 7 && std::get<1>(check_pos2) >= 0 && std::get<1>(check_pos2) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos2 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos2);

				}

				if (std::get<0>(check_pos3) >= 0 && std::get<0>(check_pos3) <= 7 && std::get<1>(check_pos3) >= 0 && std::get<1>(check_pos3) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos3 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos3);

				}

				if (std::get<0>(check_pos4) >= 0 && std::get<0>(check_pos4) <= 7 && std::get<1>(check_pos4) >= 0 && std::get<1>(check_pos4) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos4 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos4);

				}

				if (std::get<0>(check_pos5) >= 0 && std::get<0>(check_pos5) <= 7 && std::get<1>(check_pos5) >= 0 && std::get<1>(check_pos5) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos5 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos5);

				}

				if (std::get<0>(check_pos6) >= 0 && std::get<0>(check_pos6) <= 7 && std::get<1>(check_pos6) >= 0 && std::get<1>(check_pos6) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos6 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos6);

				}

				if (std::get<0>(check_pos7) >= 0 && std::get<0>(check_pos7) <= 7 && std::get<1>(check_pos7) >= 0 && std::get<1>(check_pos7) <= 7) {

					bool test = false;

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos7 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true;

						}

					}

					if (!test)
						legal_moves.push_back(check_pos7);

				}

				if (std::get<0>(check_pos8) >= 0 && std::get<0>(check_pos8) <= 7 && std::get<1>(check_pos8) >= 0 && std::get<1>(check_pos8) <= 7) {

					bool test = false; 

					for (int j = 0; j < this->get_pieces().size(); j++) {

						if (check_pos8 == this->get_pieces()[j]->pos && this->get_pieces()[i]->color == this->get_pieces()[j]->color) {

							test = true; 

						}

					}

					if (!test)
						legal_moves.push_back(check_pos8);

				}
			
			}

			else if (this->get_pieces()[i]->name == "Rook") {
				
				// Right
				for (int k = 0; k < 8; k++) {
					for (int j = 0; j < this->get_pieces().size(); j++) {

						std::tuple<int, int> check_pos = std::make_tuple(std::get<0>(this->get_pieces()[i]->pos) + k, std::get<1>(this->get_pieces()[i]->pos));

						if (check_pos == this->get_pieces()[j]->pos && this->get_pieces()[i]->color != this->get_pieces()[j]->color && check_pos != this->get_pieces()[i]->pos) {

							legal_moves.push_back(check_pos);
							goto rook_right_stop;

						}

						else if (check_pos )

					}
					if (test_pos == engine->get_pieces()[j]->pos && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						goto right_stop;

					}
					else if (std::get<0>(check_pos) == std::get<0>(engine->get_pieces()[j]->pos) && std::get<1>(check_pos) != std::get<1>(engine->get_pieces()[j]->pos) && test_pos != engine->get_pieces()[i]->pos) {

						//std::cout << std::get<0>(check_pos) << ", " << std::get<1>(check_pos) << std::endl;
						attacked_squares.push_back(check_pos);
						int counter = 0;
						for (int h = 0; h < attacked_squares.size(); h++) {

							if (check_pos == attacked_squares[h]) {

								counter += 1;

							}

						}
				}

			}

		}

	}

	return legal_moves;

}

SDL_Window* Engine::get_window() {

	return window; 

}

SDL_Renderer* Engine::get_renderer() {

	return renderer; 

}

int Engine::get_width() {

	return width; 

}

int Engine::get_height() {

	return height;

}

bool Engine::get_running() {

	return running;

}

void Engine::close() {

	running = false; 

	return; 

}

bool Engine::mouse_click(SDL_Rect rect, std::tuple<int, int> mouse) {

	if (std::get<0>(mouse) > rect.x && std::get<0>(mouse) < rect.x + rect.w && std::get<1>(mouse) > rect.y && std::get<1>(mouse) < rect.y + rect.h) {

		return true;

	}

	else {

		return false;

	}

}

bool Engine::collision(SDL_Rect rect1, SDL_Rect rect2) {

	if (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x&& rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y) {

		return true;

	}

	else {

		return false;

	}

}


bool Engine::collision_range(SDL_Rect rect1, SDL_Rect rect2, int x, int y) {

	if (rect1.x < rect2.x + rect2.w + x && rect1.x + rect1.w > rect2.x - x && rect1.y < rect2.y + rect2.h + y && rect1.y + rect1.h > rect2.y - y) {

		return true;

	}
	else
		return false;

}

std::vector<Pieces*> Engine::get_pieces() {

	return this->pieces;

}

void Engine::destroy_piece(int index) {

	this->pieces.erase(this->pieces.begin() + index);

}

Engine::~Engine() {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

}