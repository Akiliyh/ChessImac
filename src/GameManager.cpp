#include "GameManager.hpp"
#include <iostream>
#include "Pieces.hpp"

int GameManager::get_turn() const
{
    return this->m_turn;  
};

void GameManager::add_turn()
{
    this->m_turn++;
}

bool GameManager::is_white_turn() const
{
    return this->m_turn % 2 == 0;
}

void GameManager::play_game()
{
    int test_move {0};
    while (this->get_turn() < 100) {
        if (this->is_white_turn()) {
            std::cout << "White move :" << std::endl; 
            std::cout << "turn :" <<this->get_turn()<< std::endl;
            this->add_turn();
        }
        else {
            std::cout << "Black move :" << std::endl;
            std::cout << "turn :" <<this->get_turn()<< std::endl;
            this->add_turn();
        }
    }
}

void GameManager::player_move(const PieceColor& player_color)
{
    //Seul les pieces de la couleur player_color (White ou Black) peuvent être sélectionnées
    
};