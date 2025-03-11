#include "../include/main.hpp"



sf::Color getPieceColor(int piece) {
    static const std::unordered_map<int, sf::Color> pieceColors = {
        {1, sf::Color(255, 102, 102)},   
        {2, sf::Color(102, 255, 102)},   
        {3, sf::Color(102, 102, 255)},   
        {4, sf::Color(255, 255, 102)},   
        {5, sf::Color(255, 102, 255)},   
        {6, sf::Color(102, 255, 255)},   
        {7, sf::Color(255, 178, 102)},   
        {8, sf::Color(178, 102, 255)},   
        {9, sf::Color(102, 178, 255)},   
        {10, sf::Color(160, 160, 160)}   
    };

    if (piece % 10 != 0) piece %= 10;
    return pieceColors.find(piece)->second;
}


std::vector<std::pair<int, int>> getClassPositions(const std::vector<std::vector<int>>& state, int tag) {
    std::vector<std::pair<int, int>> positions;
    for (size_t i = 0; i < state.size(); i++) {
        for (size_t j = 0; j < state[i].size(); j++) {
            if (state[i][j] == tag) {
                positions.push_back({static_cast<int>(i), static_cast<int>(j)});
            }
        }
    }
    return positions;
}


std::pair<int, int> getTopLeft(const std::vector<std::vector<int>>& state, int tag) {
    auto positions = getClassPositions(state, tag);
    if (positions.empty()) return {0, 0};
    int minRow = BOARD_SIZE, minCol = BOARD_SIZE;
    for (const auto& pos : positions) {
        if (pos.first < minRow) minRow = pos.first;
        if (pos.second < minCol) minCol = pos.second;
    }
    return {minCol, minRow};
}


std::string stateToString(const std::vector<std::vector<int>>& state) {
    std::stringstream ss;
    for (const auto& row : state) {
        for (auto cell : row) {
            ss << cell << ",";
        }
        ss << ";";
    }
    return ss.str();
}


bool canMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc) {
    auto positions = getClassPositions(state, piece);
    std::vector<std::vector<int>> temp = state;
    for (auto& pos : positions) {
        temp[pos.first][pos.second] = 0;
    }
    for (auto& pos : positions) {
        int newR = pos.first + dr;
        int newC = pos.second + dc;
        if (newR < 0 || newR >= BOARD_SIZE || newC < 0 || newC >= BOARD_SIZE)
            return false;
        if (temp[newR][newC] != 0)
            return false;
    }
    return true;
}


std::vector<std::vector<int>> applyMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc) {
    auto positions = getClassPositions(state, piece);
    std::vector<std::vector<int>> newState = state;
    // Убираем объект с исходной позиции
    for (auto& pos : positions) {
        newState[pos.first][pos.second] = 0;
    }
    // Записываем объект в новом положении
    for (auto& pos : positions) {
        int newR = pos.first + dr;
        int newC = pos.second + dc;
        newState[newR][newC] = piece;
    }
    return newState;
}