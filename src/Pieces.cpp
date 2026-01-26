#include "Pieces.hpp"
#include <vector>

Rook::Rook(const int position, const bool white): Piece(position, white) {};

std::vector<int> Rook::get_moves(std::vector<Piece*>& board){
    std::vector<int> free_case;
    int i {};
    // std::vector<int> take_case;

    // std::vector<std::vector<int>> moves;

    for (int i = m_position + 8 ; i < 64 ; i += 8) {
        int next_case {i};

        if (board[next_case] == nullptr) {
            // board[m_position + 8] => color space free;
            free_case.push_back(next_case);
        } 
        else 
        {
            if (this->m_white != board[next_case]->m_white) 
            {                    
                // board[m_position + 8] => color space taken;
            free_case.push_back(next_case);
            }
        }
    }

    for (int i = m_position - 8 ; i >= 0 ; i -= 8) {
    int next_case {i};
        if (board[next_case] == nullptr) {
            // board[m_position - 8] => color space free;
            free_case.push_back(next_case);
        } 
        else 
        {
            if (this->m_white != board[next_case]->m_white) {                    
                // board[m_position - 8] => color space taken;
            free_case.push_back(next_case);
            }
        }
    }

    i = m_position + 1;
    while (i < 64 && (i / 8) == (m_position / 8)) {
        if (board[i] == nullptr) {
            free_case.push_back(i);
        } 
        else {
            if (board[i]->m_white != m_white) {
                free_case.push_back(i); // capture
            }
            break; // on s'arrête dans tous les cas
        }
        i++;
    }

    i = m_position - 1;
    while (i >= 0 && (i / 8) == (m_position / 8)) {
        if (board[i] == nullptr) {
            free_case.push_back(i);
        } 
        else {
            if (board[i]->m_white != m_white) {
                free_case.push_back(i);
            }
            break;
        }
        i--;
    }


    // moves.push_back(free_case);
    // moves.push_back(take_case);

    return free_case;
}

Bishop::Bishop(const int position, const bool white): Piece(position, white) {};

std::vector<int> Bishop::get_moves(std::vector<Piece*>& board){
    std::vector<int> free_case;
    int i {};
    int prev {};

    i = m_position + 9;
    prev = m_position;

    while (i < 64 && abs((i % 8) - (prev % 8)) == 1) {
        if (!board[i]) {
            free_case.push_back(i);
        } else {
            if (board[i]->m_white != m_white)
                free_case.push_back(i);
            break;
        }
        prev = i;
        i += 9;
    }

    i = m_position - 9;
    prev = m_position;

    while (i >= 0 && abs((i % 8) - (prev % 8)) == 1) {
        if (!board[i]) {
            free_case.push_back(i);
        } else {
            if (board[i]->m_white != m_white)
                free_case.push_back(i);
            break;
        }
        prev = i;
        i -= 9;
    }


    i = m_position + 7;
    prev = m_position;

    while (i < 64 && abs((i % 8) - (prev % 8)) == 1) {
        if (!board[i]) {
            free_case.push_back(i);
        } else {
            if (board[i]->m_white != m_white)
                free_case.push_back(i);
            break;
        }
        prev = i;
        i += 7;
    }

    i = m_position - 7;
    prev = m_position;

    while (i >= 0 && abs((i % 8) - (prev % 8)) == 1) {
        if (!board[i]) {
            free_case.push_back(i);
        } else {
            if (board[i]->m_white != m_white)
                free_case.push_back(i);
            break;
        }
        prev = i;
        i -= 7;
    }


    return free_case;
};