#include "../include/main.hpp"

int heuristic(const std::vector<std::vector<int>>& state) {
    int total = 0;
    for (auto tag : classes) {
        auto current = getTopLeft(state, tag);
        auto target = targetTopLeft[tag];
        total += std::abs(current.first - target.first) + std::abs(current.second - target.second);
    }
    return total;
}



std::vector<Node*> getNeighbors(Node* current) {
    std::vector<Node*> neighbors;
    // Возможные направления: (dr, dc) и название направления
    std::vector<std::pair<std::pair<int, int>, std::string>> directions = {
        {{-1, 0}, "↑"},
        {{1, 0}, "↓"},
        {{0, -1}, "←"},
        {{0, 1}, "→"}
    };
    // Для каждого объекта из глобального набора
    for (int piece : classes) {
        for (auto& dir : directions) {
            int dr = dir.first.first;
            int dc = dir.first.second;
            if (canMove(current->state, piece, dr, dc)) {
                auto newState = applyMove(current->state, piece, dr, dc);
                Node* neighbor = new Node(newState, current->g + 1, heuristic(newState), piece, dir.second, current);
                neighbors.push_back(neighbor);
            }
        }
    }
    
    return neighbors;
}


std::vector<Node*> a_star_solution() {
    Node* start = new Node(initialField, 0, heuristic(initialField));
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    openSet.push(start);
    std::unordered_set<std::string> closedSet;
    Node* solution = nullptr;

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->state == targetField) {
            solution = current;
            break;
        }

        std::string stateKey = stateToString(current->state);
        if (closedSet.find(stateKey) != closedSet.end())
            continue;
        closedSet.insert(stateKey);

        auto neighbors = getNeighbors(current);
        for (auto neighbor : neighbors) {
            std::string neighborKey = stateToString(neighbor->state);
            if (closedSet.find(neighborKey) == closedSet.end()) {
                openSet.push(neighbor);
            } else {
                delete neighbor;
            }
        }
    }

    std::vector<Node*> path;
    if (solution == nullptr) {
        std::cout << "Решение не найдено.\n";
        return path;
    }

    // Восстанавливаем путь от решения до начального состояния
    for (Node* node = solution; node != nullptr; node = node->parent) {
        path.push_back(node);
    }
    std::reverse(path.begin(), path.end());
    printA_star_path(path);
    return path;
}